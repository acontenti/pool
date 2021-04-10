#include "pool.hpp"
#include <poolstd.hpp>
#include <callable.hpp>
#include <util/errors.hpp>

using namespace pool;

shared_ptr<Class> pool::ClassClass = nullptr;
shared_ptr<Class> pool::ObjectClass = nullptr;
shared_ptr<Class> pool::BoolClass = nullptr;
shared_ptr<Class> pool::NumberClass = nullptr;
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

shared_ptr<Function> Object::findMethod(const string &methodName) const {
	if (cls) {
		if (auto object = cls->findInClass(methodName)) {
			if (auto executable = dynamic_pointer_cast<Function>(object->getValue())) {
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

string Object::getRepr() const {
	return getClass()->name + "@" + to_string(id);
}

bool Object::instanceOf(const shared_ptr<Class> &_class) const {
	if (cls) {
		return cls == _class || cls->subclassOf(_class);
	} else return _class == ObjectClass;
}

shared_ptr<Class> Object::getClass() const {
	return cls ? cls : as<Class>();
}

Class::Class(const shared_ptr<Context> &context, creator_t creator, string name, shared_ptr<Class> super)
		: Object(context, ClassClass), creator(move(creator)), name(move(name)), super(move(super)) {}

shared_ptr<Class> Class::extend(const creator_t &_creator, const string &className, const shared_ptr<Block> &block, const Location &location) const {
	const auto &cls = ClassClass->newInstance(context, location, {}, ClassData{_creator, className, this->as<Class>()})->as<Class>();
	if (block) {
		block->execute(location);
		for (const auto&[name, value] : *block->context) {
			cls->context->set(name, value->getValue(), value->isImmutable());
		}
	}
	return cls;
}

shared_ptr<Object> Class::newInstance(const shared_ptr<Context> &parent, const Location &location, const vector<shared_ptr<Object>> &other, const any &data) const {
	auto ptr = creator(Context::create(parent), data);
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

bool Class::subclassOf(const shared_ptr<Class> &other) const {
	return super && (super == other || super->subclassOf(other));
}

bool Class::superclassOf(const shared_ptr<Class> &other) const {
	return other->super && (this == other->super.get() || superclassOf(other->super));
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

CodeFunction::CodeFunction(const shared_ptr<Context> &context, const vector<Param> &params, const vector<shared_ptr<Callable>> &calls)
		: Function(context, params), calls(calls) {}

shared_ptr<Object> CodeFunction::execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
	shared_ptr<Object> returnValue = Void;
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
			if (param.type && !value->instanceOf(param.type)) {
				throw compile_error(
						"Argument for function parameter '" + param.id + "' must be of class '" + param.type->name
						+ "', but value of class '" + value->getClass()->name + "' was given", location);
			} else context->set(param.id, value);
		} else {
			context->set(param.id, Array::newInstance(context, location, {args.begin() + i, args.end()}));
		}
	}
	for (auto &call: calls) {
		returnValue = call->invoke(context);
	}
	return returnValue;
}

shared_ptr<Object> CodeFunction::newInstance(const shared_ptr<Context> &context, const Location &location, const vector<Param> &params, const vector<shared_ptr<Callable>> &calls) {
	return FunctionClass->newInstance(context, location, {}, CodeFunctionData{params, calls});
}

NativeFunction::NativeFunction(const shared_ptr<Context> &context, const vector<Param> &params, method_t code)
		: Function(context, params), code(move(code)) {}

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
		if (!param.rest && param.type) {
			const auto &value = other[i];
			if (!value->instanceOf(param.type)) {
				throw compile_error(
						"Argument for function parameter '" + param.id + "' must be of class '" + param.type->name
						+ "', but value of class '" + value->getClass()->name + "' was given", location);
			}
		}
	}
	auto returnValue = code(self, other, location);
	return returnValue;
}

shared_ptr<NativeFunction> NativeFunction::newInstance(const vector<Param> &params, const method_t &code) {
	return make_shared<NativeFunction>(Context::global, params, code);
}

Block::Block(const shared_ptr<Context> &context, const vector<shared_ptr<Callable>> &calls)
		: Executable(context, BlockClass), calls(calls) {}

shared_ptr<Object> Block::execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
	shared_ptr<Object> returnValue = Void;
	for (auto &call: calls) {
		returnValue = call->invoke(context);
	}
	return returnValue;
}

shared_ptr<Object> Block::execute(const Location &location) {
	return execute(shared_from_this(), {}, location);
}

shared_ptr<Object> Block::newInstance(const shared_ptr<Context> &context, const Location &location, const vector<shared_ptr<Callable>> &calls) {
	return BlockClass->newInstance(context, location, {}, calls);
}

Array::Array(const shared_ptr<Context> &context, const vector<shared_ptr<Object>> &values)
		: Object(context, ArrayClass), values(values) {}

shared_ptr<Object> Array::newInstance(const shared_ptr<Context> &context, const Location &location, const vector<shared_ptr<Object>> &values) {
	return ArrayClass->newInstance(context, location, {}, values);
}

void initializeContext() {
	Context::global = Context::create(nullptr);
	Context::global->set("import", NativeFunction::newInstance({{"moduleName", StringClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
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
	ClassClass = Class::CREATOR(Context::create(Context::global), Class::ClassData{Class::CREATOR, "Class", nullptr}); // ObjectClass is not yet created, so we pass nullptr as super
	ObjectClass = Class::CREATOR(Context::create(Context::global), Class::ClassData{Object::CREATOR, "Object", nullptr}); // ObjectClass is the base class, so it has no super
	ClassClass->super = ObjectClass; // Now ObjectClass exists, so we can assign it to ClassClass->super
	BoolClass = ObjectClass->extend(Bool::CREATOR, "Bool", nullptr, Location::UNKNOWN);
	NumberClass = ObjectClass->extend(Object::CREATOR, "Number", nullptr, Location::UNKNOWN);
	IntegerClass = NumberClass->extend(Integer::CREATOR, "Integer", nullptr, Location::UNKNOWN);
	DecimalClass = NumberClass->extend(Decimal::CREATOR, "Decimal", nullptr, Location::UNKNOWN);
	StringClass = ObjectClass->extend(String::CREATOR, "String", nullptr, Location::UNKNOWN);
	ArrayClass = ObjectClass->extend(Array::CREATOR, "Array", nullptr, Location::UNKNOWN);
	BlockClass = ObjectClass->extend(Block::CREATOR, "Block", nullptr, Location::UNKNOWN);
	FunctionClass = ObjectClass->extend(CodeFunction::CREATOR, "Function", nullptr, Location::UNKNOWN);
	VoidClass = ObjectClass->extend(Object::CREATOR, "Void", nullptr, Location::UNKNOWN);
	NothingClass = ObjectClass->extend(Object::CREATOR, "Nothing", nullptr, Location::UNKNOWN);
	Void = VoidClass->newInstance(Context::global, Location::UNKNOWN, {}, VoidClass);
	Null = NothingClass->newInstance(Context::global, Location::UNKNOWN, {}, NothingClass);
	True = BoolClass->newInstance(Context::global, Location::UNKNOWN, {}, true)->as<Bool>();
	False = BoolClass->newInstance(Context::global, Location::UNKNOWN, {}, false)->as<Bool>();
}

void pool::initializeStdLib() {
	initializeContext();
	initializeBaseObjects();
}
