#include "pool_private.hpp"
#include <poolstd.hpp>
#include <natives.hpp>
#include <callable.hpp>
#include <util/strings.hpp>
#include <iostream>
#include <utility>

using namespace pool;

shared_ptr<Class> pool::ClassClass = nullptr;
shared_ptr<Class> pool::ObjectClass = nullptr;
shared_ptr<Class> pool::SymbolClass = nullptr;
shared_ptr<Class> pool::ModuleClass = nullptr;
shared_ptr<Class> pool::BoolClass = nullptr;
shared_ptr<Class> pool::StringClass = nullptr;
shared_ptr<Class> pool::ArrayClass = nullptr;
shared_ptr<Class> pool::FunctionClass = nullptr;
shared_ptr<Class> pool::NothingClass = nullptr;
shared_ptr<Object> pool::Null = nullptr;
shared_ptr<Object> pool::True = nullptr;
shared_ptr<Object> pool::False = nullptr;

Object::Object(const shared_ptr<Context> &context, shared_ptr<Class> cls)
		: context(context), cls(move(cls)), id(reinterpret_cast<intptr_t>(this)) {}

shared_ptr<Function> Object::findMethod(const string &methodName) const {
	if (auto var = getClass()->findInClass(methodName)) {
		if (auto executable = dynamic_pointer_cast<Function>(var->getValue())) {
			return executable;
		}
	}
	return nullptr;
}

shared_ptr<Variable> Object::find(const string &name) const {
	if (auto local = context->findLocal(name)) {
		return local;
	}
	return getClass()->findInClass(name);
}

shared_ptr<Variable> Object::findLocal(const string &name) const {
	return context->findLocal(name);
}

void Object::remove(const string &name) {
	context->remove(name);
}

string Object::getDefaultRepr() const {
	return getClass()->name + "@" + to_string_base<62>(id);
}

string Object::getRepr(const Location &location) {
	if (auto init = findMethod("getRepr")) {
		const auto &result = init->execute(shared_from_this(), {}, location);
		if (result->instanceOf(StringClass)) {
			return result->as<String>()->value;
		}
	}
	return getDefaultRepr();
}

string Object::toString(const Location &location) {
	if (const auto &method = findMethod("toString")) {
		const auto &result = method->execute(shared_from_this(), {}, location);
		if (result->instanceOf(StringClass)) {
			return result->as<String>()->value;
		}
	}
	return getRepr(location);
}

bool Object::instanceOf(const shared_ptr<Class> &_class) const {
	const auto &thisClass = getClass();
	return thisClass == _class || thisClass->subclassOf(_class);
}

bool Object::instanceOf(const string &className) const {
	const auto &_class = context->findClass(className);
	return _class && instanceOf(_class);
}

shared_ptr<Class> Object::getClass() const {
	return cls ? cls : as<Class>();
}

string Object::getContextInfo(const Location &location) const {
	stringstream ss;
	ss << "{ ";
	if (!context->empty()) {
		for (const auto &[name, value]: *context) {
			ss << name << ":" << value->getValue()->getRepr(location) << ", ";
		}
	}
	ss << "}";
	return ss.str();
}

Class::Class(const shared_ptr<Context> &context, creator_t creator, string name, shared_ptr<Class> super)
		: Object(context, ClassClass), creator(move(creator)), name(move(name)), super(move(super)) {}

shared_ptr<Class> Class::extend(const creator_t &_creator, const string &className, const shared_ptr<Function> &block, const Location &location) const {
	const auto &cls = ClassClass->newInstance(context, location, {}, ClassData{_creator, className, this->as<Class>()})->as<Class>();
	if (block) {
		block->context->set("class", cls, true, location);
		block->context->set("super", shared_from_this()->as<Class>(), true, location);
		block->execute(block, {}, location);
		for (const auto &[_name, value]: *block->context) {
			cls->context->set(_name, value->getValue(), value->isImmutable(), location);
		}
	}
	return cls;
}

shared_ptr<Object> Class::newInstance(const shared_ptr<Context> &parent, const Location &location, const vector<shared_ptr<Object>> &args, const any &data) const {
	auto ptr = creator(Context::create(parent), this->as<Class>(), data);
	if (auto init = ptr->findMethod("init")) {
		init->execute(ptr, args, location);
	}
	return ptr;
}

shared_ptr<Variable> Class::find(const string &id) const {
	if (auto local = findInClass(id)) {
		return local;
	}
	if (cls) { // if not ClassClass
		return cls->find(id);
	}
	return nullptr;
}

shared_ptr<Variable> Class::findInClass(const string &id) const {
	if (auto local = context->findLocal(id)) {
		return local;
	}
	if (super) {
		return super->findInClass(id);
	}
	return nullptr;
}

bool Class::subclassOf(const shared_ptr<Class> &other) const {
	return super && (super == other || super->subclassOf(other));
}

bool Class::superclassOf(const shared_ptr<Class> &other) const {
	return other->super && (this == other->super.get() || superclassOf(other->super));
}

Symbol::Symbol(const shared_ptr<Context> &context, const shared_ptr<Class> &cls, string id)
		: Executable(context, cls), id(move(id)) {}

any Symbol::getValue(const Location &location) const {
	auto value = Natives::get().find(id);
	if (value.has_value()) {
		return value;
	} else {
		throw compile_error("Symbol " + id + " not found", location);
	}
}

shared_ptr<Object> Symbol::execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
	try {
		return any_cast<shared_ptr<Object>>(getValue(location));
	} catch (bad_any_cast &exception) {
		throw compile_error("Symbol must refer to a shared_ptr<Object>", location);
	}
}

shared_ptr<Object> Symbol::newInstance(const shared_ptr<Context> &context, const Location &location, const string &id) {
	return SymbolClass->newInstance(context, location, {}, id);
}

Module::Module(const shared_ptr<Context> &context, const shared_ptr<Class> &cls, string id)
		: Object(context, cls), id(move(id)) {}

void Module::load(const shared_ptr<Function> &body, const Location &location) {
	exported = ObjectClass->newInstance(context, location, {}, nullptr);
	body->context->set("__module__", shared_from_this(), true);
	body->execute(body, {exported}, location);
}

void Module::inject(const shared_ptr<Context> &dest, const Location &location) const {
	for (const auto &[k, v]: *exported->context) {
		dest->set(k, v->getValue(), v->isImmutable(), location);
	}
}

shared_ptr<Variable> Module::find(const string &name) const {
	if (const auto &result = Object::find(name)) {
		return result;
	}
	return exported->find(name);
}

shared_ptr<Object> Module::newInstance(const shared_ptr<Context> &context, const Location &location, const string &id, const shared_ptr<Function> &body) {
	auto module = ModuleClass->newInstance(context, location, {}, id)->as<Module>();
	module->load(body, location);
	return module;
}

string Module::getContextInfo(const Location &location) const {
	const auto &first = Object::getContextInfo(location);
	const auto &second = exported->getContextInfo(location);
	return first.substr(0, first.size() - 2) + second.substr(1);
}

Nothing::Nothing(const shared_ptr<Context> &context, const shared_ptr<Class> &cls)
		: Object(context, cls) {}

Bool::Bool(const shared_ptr<Context> &context, const shared_ptr<Class> &cls, bool value)
		: Object(context, cls), value(value) {}

Integer::Integer(const shared_ptr<Context> &context, const shared_ptr<Class> &cls, long long int value)
		: Object(context, cls), value(value) {}

shared_ptr<Object> Integer::newInstance(const shared_ptr<Context> &context, const Location &location, const long long int &value) {
	if (const auto &cls = Context::global->findClass("Integer")) {
		return cls->newInstance(context, location, {}, value);
	} else throw compile_error("Class Integer is not defined", location);
}

Decimal::Decimal(const shared_ptr<Context> &context, const shared_ptr<Class> &cls, long double value)
		: Object(context, cls), value(value) {}

shared_ptr<Object> Decimal::newInstance(const shared_ptr<Context> &context, const Location &location, const long double &value) {
	if (const auto &cls = Context::global->findClass("Decimal")) {
		return cls->newInstance(context, location, {}, value);
	} else throw compile_error("Class Decimal is not defined", location);
}

String::String(const shared_ptr<Context> &context, const shared_ptr<Class> &cls, string value)
		: Object(context, cls), value(move(value)) {}

shared_ptr<Object> String::newInstance(const shared_ptr<Context> &context, const Location &location, const string &value) {
	return StringClass->newInstance(context, location, {}, value);
}

Executable::Executable(const shared_ptr<Context> &context, const shared_ptr<Class> &cls)
		: Object(context, cls) {}

Function::Function(const shared_ptr<Context> &context, const shared_ptr<Class> &cls, const vector<Param> &params)
		: Executable(context, cls), params(params) {}

CodeFunction::CodeFunction(const shared_ptr<Context> &context, const shared_ptr<Class> &cls, const vector<Param> &params, const vector<shared_ptr<Callable>> &calls)
		: Function(context, cls, params), calls(calls) {}

shared_ptr<Object> CodeFunction::execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
	vector<shared_ptr<Object>> args;
	if (self.get() != this) {
		args.reserve(other.size() + 1);
		args.push_back(self);
	} else {
		args.reserve(other.size());
	}
	args.insert(args.end(), other.begin(), other.end());
	const auto hasRest = !params.empty() && params.back().rest;
	const auto paramsSize = params.size();
	if (hasRest) {
		if (args.size() < paramsSize - 1)
			throw compile_error("Function takes at least " + to_string(paramsSize - 1)
								+ " arguments, but " + to_string(args.size()) + " were given", location);
	} else if (paramsSize != args.size()) {
		throw compile_error("Function takes " + to_string(paramsSize)
							+ " arguments, but " + to_string(args.size()) + " were given", location);
	}
	for (int i = 0; i < paramsSize; ++i) {
		const auto &param = params[i];
		if (!param.rest) {
			const auto &value = args[i];
			if (param.type) {
				if (!value->instanceOf(param.type)) {
					throw compile_error(
							"Argument for function parameter '" + param.id + "' must be of class '" + param.type->name
							+ "', but value of class '" + value->getClass()->name + "' was given", location);
				}
			} else if (!param.typeName.empty()) {
				if (!value->instanceOf(param.typeName)) {
					throw compile_error(
							"Argument for function parameter '" + param.id + "' must be of class '" + param.typeName
							+ "', but value of class '" + value->getClass()->name + "' was given", location);
				}
			}
			context->set(param.id, value, false, location);
		} else {
			context->set(param.id, Array::newInstance(context, location, {args.begin() + i, args.end()}),
						 false, location);
		}
	}
	shared_ptr<Object> returnValue = Null;
	try {
		for (auto &call: calls) {
			returnValue = call->invoke(context);
		}
	} catch (const Function::return_exception &exception) {
		if (exception.self == shared_from_this()) {
			returnValue = exception.value;
		} else {
			throw exception;
		}
	}
	return returnValue;
}

shared_ptr<Object> CodeFunction::newInstance(const shared_ptr<Context> &context, const Location &location, const vector<Param> &params, const vector<shared_ptr<Callable>> &calls) {
	auto ptr = FunctionClass->newInstance(context, location, {}, CodeFunctionData{params, calls});
	ptr->context->set("_", ptr, true);
	return ptr;
}

NativeFunction::NativeFunction(const shared_ptr<Context> &context, const shared_ptr<Class> &cls, const vector<Param> &params, method_t code)
		: Function(context, cls, params), code(move(code)) {}

shared_ptr<Object> NativeFunction::execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
	bool thisIsSelf = self.get() == this;
	bool hasRest = !params.empty() && params.back().rest;
	auto otherSize = thisIsSelf ? other.size() : other.size() + 1;
	auto paramsSize = params.size();
	if (hasRest) {
		if (otherSize < paramsSize - 1)
			throw compile_error("Function takes at least " + to_string(paramsSize - 1)
								+ " arguments, but " + to_string(otherSize) + " were given", location);
	} else if (paramsSize != otherSize) {
		throw compile_error("Function takes " + to_string(paramsSize)
							+ " arguments, but " + to_string(otherSize) + " were given", location);
	}
	for (int i = 0, delta = thisIsSelf ? 0 : 1; i < paramsSize - delta; ++i) {
		const auto &param = params[i + delta];
		if (!param.rest) {
			if (param.type || !param.typeName.empty()) {
				const auto &value = other[i];
				if (param.type) {
					if (!value->instanceOf(param.type)) {
						throw compile_error(
								"Argument for function parameter '" + param.id + "' must be of class '" +
								param.type->name
								+ "', but value of class '" + value->getClass()->name + "' was given", location);
					}
				} else if (!value->instanceOf(param.typeName)) {
					throw compile_error(
							"Argument for function parameter '" + param.id + "' must be of class '" +
							param.typeName
							+ "', but value of class '" + value->getClass()->name + "' was given", location);
				}
			}
		}
	}
	auto returnValue = code(self, other, location);
	return returnValue;
}

shared_ptr<NativeFunction> NativeFunction::newInstance(const vector<Param> &params, const method_t &code) {
	return make_shared<NativeFunction>(Context::global, FunctionClass, params, code);
}

Array::Array(const shared_ptr<Context> &context, const shared_ptr<Class> &cls, const vector<shared_ptr<Object>> &values)
		: Object(context, cls), values(values) {}

shared_ptr<Object> Array::newInstance(const shared_ptr<Context> &context, const Location &location, const vector<shared_ptr<Object>> &values) {
	return ArrayClass->newInstance(context, location, {}, values);
}

void initializeBaseObjects() {
	ClassClass = Class::CREATOR(Context::create(Context::global), nullptr, Class::ClassData{Class::CREATOR, "Class", nullptr}); // ObjectClass is not yet created, so we pass nullptr as super
	ObjectClass = Class::CREATOR(Context::create(Context::global), ClassClass, Class::ClassData{Object::CREATOR, "Object", nullptr}); // ObjectClass is the base class, so it has no super
	const_pointer_cast<Class>(ClassClass->super) = ObjectClass; // Now ObjectClass exists, so we can assign it to ClassClass->super
	SymbolClass = ObjectClass->extend(Symbol::CREATOR, "Symbol", nullptr, Location::UNKNOWN);
	ModuleClass = ObjectClass->extend(Module::CREATOR, "Module", nullptr, Location::UNKNOWN);
	BoolClass = ObjectClass->extend(Bool::CREATOR, "Bool", nullptr, Location::UNKNOWN);
	StringClass = ObjectClass->extend(String::CREATOR, "String", nullptr, Location::UNKNOWN);
	ArrayClass = ObjectClass->extend(Array::CREATOR, "Array", nullptr, Location::UNKNOWN);
	FunctionClass = ObjectClass->extend(CodeFunction::CREATOR, "Function", nullptr, Location::UNKNOWN);
	NothingClass = ObjectClass->extend(Nothing::CREATOR, "Nothing", nullptr, Location::UNKNOWN);
	Null = NothingClass->newInstance(Context::global, Location::UNKNOWN, {}, nullptr);
	True = BoolClass->newInstance(Context::global, Location::UNKNOWN, {}, true);
	False = BoolClass->newInstance(Context::global, Location::UNKNOWN, {}, false);
}

void initializeContext() {
	Context::global = Context::create(nullptr);
	initializeBaseObjects();
	Context::global->set("Object", ObjectClass, true);
	Context::global->set("Class", ClassClass, true);
	Context::global->set("Symbol", SymbolClass, true);
	Context::global->set("Module", ModuleClass, true);
	Context::global->set("Bool", BoolClass, true);
	Context::global->set("String", StringClass, true);
	Context::global->set("Array", ArrayClass, true);
	Context::global->set("Function", FunctionClass, true);
	Context::global->set("Nothing", NothingClass, true);
	Context::global->set("null", Null, true);
	Context::global->set("true", True, true);
	Context::global->set("false", False, true);
	Context::global->set("newClass", NativeFunction::newInstance({{"name", StringClass}, {"creator", SymbolClass}, {"super", ClassClass}, {"body", FunctionClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) -> shared_ptr<Object> {
		try {
			const auto &name = other[0]->as<String>()->value;
			const auto &creator = any_cast<Class::creator_t>(other[1]->as<Symbol>()->getValue(location));
			const auto &super = other[2]->as<Class>();
			const auto &block = other[3]->as<Function>();
			return super->extend(creator, name, block, location);
		} catch (bad_any_cast &exception) {
			throw compile_error("Creator Symbol must refer to a Class:creator_t", location);
		}
	}), true);
	Context::global->set("import", NativeFunction::newInstance({{"moduleName", StringClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
		const auto &moduleName = other[0]->as<String>()->value;
		try {
			return PoolVM::get()->execute(moduleName)->as<Object>();
		} catch (const compile_error &e) {
			cerr << e;
			throw compile_error("Cannot import module '" + moduleName + "': " + e.what(), location);
		}
	}), true);
	Context::global->set("importStd", NativeFunction::newInstance({}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
		try {
			static shared_ptr<Module> module;
			if (!module) {
				module = PoolVM::get()->execute(":std");
				module->inject(Context::global, location);
			}
			return module->as<Object>();
		} catch (const compile_error &e) {
			cerr << e;
			throw compile_error("Cannot import module ':std': " + string(e), location);
		}
	}), true);
}

void pool::initializeStdLib() {
	initializeContext();
}
