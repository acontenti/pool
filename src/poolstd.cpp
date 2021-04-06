#include "poolstd_private.hpp"
#include "pool.hpp"
#include "util/errors.hpp"

using namespace pool;

shared_ptr<Class> pool::ClassClass = nullptr;
shared_ptr<Class> pool::ObjectClass = nullptr;
shared_ptr<Class> pool::BoolClass = nullptr;
shared_ptr<Class> pool::IntegerClass = nullptr;
shared_ptr<Class> pool::DecimalClass = nullptr;
shared_ptr<Class> pool::StringClass = nullptr;
shared_ptr<Class> pool::ArrayClass = nullptr;
shared_ptr<Class> pool::BlockClass = nullptr;
shared_ptr<Class> pool::FunctionClass = nullptr;
shared_ptr<Class> pool::VoidClass = nullptr;
shared_ptr<Class> pool::NothingClass = nullptr;
shared_ptr<Object> pool::Void = nullptr;
shared_ptr<Object> pool::Null = nullptr;
shared_ptr<Bool> pool::True = nullptr;
shared_ptr<Bool> pool::False = nullptr;

Object::Object(const shared_ptr<Context> &context, shared_ptr<Class> cls)
		: context(context), cls(move(cls)), id(cls ? cls->instances++ : 0) {}

string_view Object::getType() {
	return cls ? cls->name : as<Class>()->name;
}

shared_ptr<Function> Object::findMethod(const string &methodName) const {
	if (cls) {
		if (auto object = cls->findInClass(methodName)) {
			if (auto executable = dynamic_pointer_cast<Function>(object->as<Object>())) {
				return executable;
			}
		}
	}
	return nullptr;
}

shared_ptr<Variable> Object::find(const string &name) const {
	if (auto local = context->findLocal(name)) {
		return local;
	}
	if (cls) {
		return cls->findInClass(name);
	}
	return nullptr;
}

shared_ptr<Variable> Object::findLocal(const string &name) const {
	return context->findLocal(name);
}

void Object::remove(const string &name) {
	context->remove(name);
}

shared_ptr<Object> Object::getVariableValue() const {
	return const_pointer_cast<Object>(reinterpret_pointer_cast<const Variable>(shared_from_this())->getValue());
}

bool Object::isVariable() const {
	return false;
}

string Object::toString() {
	return string(getType()) + "@" + to_string(id);
}

bool Object::instanceOf(const shared_ptr<Class> &_class) {
	if (cls) {
		return cls == _class || cls->subclassOf(_class);
	} else return _class == ObjectClass;
}

template<>
shared_ptr<Variable> Object::as<Variable>() const {
	return const_pointer_cast<Variable>(reinterpret_pointer_cast<const Variable>(shared_from_this()));
}

Class::Class(const shared_ptr<Context> &context, creator_t creator, string name, shared_ptr<Class> super)
		: Object(context, ClassClass), creator(move(creator)), name(move(name)), super(move(super)) {}

shared_ptr<Class> Class::extend(const creator_t &_creator, const string &className, const shared_ptr<Block> &other, const Location &location) const {
	auto self = this->as<Class>();
	auto cls = ClassClass->newInstance(context, location, {}, _creator ? _creator
																	   : creator, className, self)->as<Class>();
	cls->context->set("super", self);
	if (other) {
		other->execute(location);
		for (auto[name, value] : *other->context) {
			cls->context->set(name, value->as<Object>());
		}
	}
	return cls;
}

shared_ptr<Object> Class::newInstance(const shared_ptr<Context> &parent, const Location &location, const vector<shared_ptr<Object>> &other, const any &a1, const any &a2, const any &a3) const {
	auto context = Context::create(parent);
	auto ptr = creator(context, a1, a2, a3);
	if (ptr->cls) {
		ptr->context->set("class", ptr->cls);
	}
	if (auto init = ptr->findMethod("init")) {
		init->execute(ptr, other, location);
	}
	return ptr;
}

shared_ptr<Variable> Class::find(const string &id) const {
	if (auto local = findInClass(id)) {
		return local;
	}
	if (cls) {
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

bool Class::subclassOf(const shared_ptr<Class> &_class) const {
	return super && (super == _class || super->subclassOf(_class));
}

bool Class::superclassOf(const shared_ptr<Class> &_class) const {
	return _class->super && (this == _class->super.get() || superclassOf(_class->super));
}

Variable::Variable(const shared_ptr<Context> &context, string name, shared_ptr<Object> value, bool immutable)
		: Object(context, nullptr), name(move(name)), value(move(value)), immutable(immutable) {
}

bool Variable::instanceOf(const shared_ptr<Class> &_class) {
	return value->instanceOf(_class);
}

void Variable::setValue(const shared_ptr<Object> &val) {
	if (!immutable)
		value = val->as<Object>();
	else throw compile_error("Cannot assign immutable variable '" + name + "'", Location::UNKNOWN);
}

bool Variable::isImmutable() const {
	return immutable;
}

void Variable::setImmutable(bool _immutable) {
	immutable = _immutable;
}

const shared_ptr<Object> &Variable::getValue() const {
	return value;
}

shared_ptr<Function> Variable::findMethod(const string &methodName) const {
	return value->findMethod(methodName);
}

shared_ptr<Variable> Variable::find(const string &id) const {
	return value->find(id);
}

shared_ptr<Variable> Variable::findLocal(const string &id) const {
	return value->findLocal(id);
}

string_view Variable::getType() {
	return value->getType();
}

bool Variable::isVariable() const {
	return true;
}

Bool::Bool(const shared_ptr<Context> &context, bool value)
		: Object(context, BoolClass), value(value) {}

Integer::Integer(const shared_ptr<Context> &context, long long int value)
		: Object(context, IntegerClass), value(value) {}

Decimal::Decimal(const shared_ptr<Context> &context, long double value)
		: Object(context, DecimalClass), value(value) {}

String::String(const shared_ptr<Context> &context, string value)
		: Object(context, StringClass), value(move(value)) {}

Executable::Executable(const shared_ptr<Context> &context, const shared_ptr<Class> &cls)
		: Object(context, cls) {}

Function::Function(const shared_ptr<Context> &context, const vector<Param> &params)
		: Executable(context, FunctionClass), params(params) {}

CodeFunction::CodeFunction(const shared_ptr<Context> &context, const vector<Param> &params, vector<PoolParser::StatementContext *> calls)
		: Function(context, params), calls(move(calls)) {}

shared_ptr<Object> CodeFunction::execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
	shared_ptr<Object> returnValue = Void;
	vector<shared_ptr<Object>> args;
	auto ptr = self->as<Object>();
	if (ptr != shared_from_this()) {
		args.reserve(other.size() + 1);
		args.push_back(ptr);
	} else {
		args.reserve(other.size());
	}
	args.insert(args.end(), other.begin(), other.end());
	if (!params.empty() && params.back().rest) {
		if (params.size() - 1 > args.size())
			throw compile_error("Function takes at least " + to_string(params.size() - 1)
								+ " arguments, but " + to_string(args.size()) + " were given", location);
	} else if (params.size() != args.size()) {
		throw compile_error("Function takes " + to_string(params.size())
							+ " arguments, but " + to_string(args.size()) + " were given", location);
	}
	for (int i = 0; i < params.size(); ++i) {
		shared_ptr<Object> value;
		auto name = params[i].id;
		if (params[i].rest) {
			vector<shared_ptr<Object>> rest;
			rest.reserve(args.size() - i);
			for (int j = i; j < args.size(); ++j) {
				rest.emplace_back(args[j]);
			}
			value = ArrayClass->newInstance(context, location, rest);
		} else {
			value = args[i];
		}
		context->set(name, value);
	}
	for (auto &call: calls) {
		if (auto exp = call->expression()) {
			returnValue = parseExpression(exp, context)->invoke();
		} else {
			returnValue = Void;
		}
	}
	return returnValue;
}

NativeFunction::NativeFunction(const shared_ptr<Context> &context, const vector<Param> &params, method_t code)
		: Function(context, params), code(move(code)) {}

shared_ptr<Object> NativeFunction::execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
	if (self->as<Object>() == shared_from_this()) {
		if (!params.empty() && params.back().rest) {
			if (params.size() - 1 > other.size())
				throw compile_error("Function takes at least " + to_string(params.size() - 1)
									+ " arguments, but " + to_string(other.size()) + " were given", location);
		} else {
			if (params.size() != other.size())
				throw compile_error("Function takes " + to_string(params.size())
									+ " arguments, but " + to_string(other.size()) + " were given", location);
		}
	} else {
		if (!params.empty() && params.back().rest) {
			if (params.size() - 1 > other.size() + 1)
				throw compile_error("Function takes at least " + to_string(params.size() - 1)
									+ " arguments, but " + to_string(other.size() + 1) + " were given", location);
		} else {
			if (params.size() != other.size() + 1)
				throw compile_error("Function takes " + to_string(params.size())
									+ " arguments, but " + to_string(other.size() + 1) + " were given", location);
		}
	}
	auto returnValue = code(self, other, location);
	return returnValue;
}

shared_ptr<NativeFunction> NativeFunction::newInstance(const vector<Param> &params, const method_t &code) {
	return make_shared<NativeFunction>(Context::global, params, code);
}

Block::Block(const shared_ptr<Context> &context, vector<PoolParser::StatementContext *> calls)
		: Executable(context, BlockClass), calls(move(calls)) {}

shared_ptr<Object> Block::execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
	shared_ptr<Object> returnValue = Void;
	for (auto &call: calls) {
		if (auto exp = call->expression()) {
			returnValue = parseExpression(exp, context)->invoke();
		} else {
			returnValue = Void;
		}
	}
	return returnValue;
}

shared_ptr<Object> Block::execute(const Location &location) {
	return execute(shared_from_this(), {}, location);
}

Array::Array(const shared_ptr<Context> &context) : Object(context, ArrayClass) {}

void initializeContext() {
	Context::global = Context::create(nullptr);
	Context::global->set("import", NativeFunction::newInstance({{"moduleName"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
		auto arg = other[0];
		if (arg->instanceOf(StringClass)) {
			auto moduleName = arg->as<String>()->value;
			try {
				auto result = PoolVM::execute(moduleName);
				if (!result) {
					throw runtime_error("execution error");
				}
			} catch (const runtime_error &e) {
				throw compile_error("Cannot import module '" + moduleName + "': " + e.what(), location);
			}
		} else throw compile_error("import argument must be a String", location);
		return Void;
	}));
}

void initializeBaseObjects() {
	ClassClass = make_shared<Class>(Context::create(Context::global), Class::CREATOR, "Class", shared_ptr<Class>(nullptr))->as<Class>(); // ObjectClass is not yet created, so we pass nullptr as super
	ObjectClass = ClassClass->newInstance(Context::global, Location::UNKNOWN, {}, static_cast<Class::creator_t>(Object::CREATOR), string("Object"), shared_ptr<Class>(nullptr))->as<Class>(); // ObjectClass is the base class, so it has no super
	ClassClass->super = ObjectClass; // Now ObjectClass exists, so we can assign it to ClassClass->super
	BoolClass = ObjectClass->extend(Bool::CREATOR, "Bool", {}, Location::UNKNOWN);
	IntegerClass = ObjectClass->extend(Integer::CREATOR, "Integer", {}, Location::UNKNOWN);
	DecimalClass = ObjectClass->extend(Decimal::CREATOR, "Decimal", {}, Location::UNKNOWN);
	StringClass = ObjectClass->extend(String::CREATOR, "String", {}, Location::UNKNOWN);
	ArrayClass = ObjectClass->extend(Array::CREATOR, "Array", {}, Location::UNKNOWN);
	BlockClass = ObjectClass->extend(Block::CREATOR, "Block", {}, Location::UNKNOWN);
	FunctionClass = ObjectClass->extend(CodeFunction::CREATOR, "Function", {}, Location::UNKNOWN);
	VoidClass = ObjectClass->extend(nullptr, "Void", {}, Location::UNKNOWN);
	NothingClass = ObjectClass->extend(nullptr, "Nothing", {}, Location::UNKNOWN);
	Void = VoidClass->newInstance(Context::global, Location::UNKNOWN, {}, VoidClass);
	Null = NothingClass->newInstance(Context::global, Location::UNKNOWN, {}, NothingClass);
	True = BoolClass->newInstance(Context::global, Location::UNKNOWN, {}, true)->as<Bool>();
	False = BoolClass->newInstance(Context::global, Location::UNKNOWN, {}, false)->as<Bool>();
}

void pool::initializeStdLib() {
	initializeContext();
	initializeBaseObjects();
}
