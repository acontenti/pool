#include "poolstd.hpp"
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
shared_ptr<Class> pool::FunClass = nullptr;
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

shared_ptr<Executable> Object::findMethod(const string &methodName) const {
	if (cls) {
		if (auto object = cls->findInClass(methodName)) {
			if (auto executable = dynamic_pointer_cast<Executable>(object->as<Object>())) {
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

template<>
shared_ptr<Variable> Object::as<Variable>() const {
	return const_pointer_cast<Variable>(reinterpret_pointer_cast<const Variable>(shared_from_this()));
}

Class::Class(const shared_ptr<Context> &context, creator_t creator, string name, shared_ptr<Class> super)
		: Object(context, ClassClass), creator(move(creator)), name(move(name)), super(move(super)) {}

shared_ptr<Class> Class::extend(const creator_t &_creator, const string &className, const shared_ptr<Block> &other, const pair<Token *, Token *> &location) const {
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

shared_ptr<Object> Class::newInstance(const shared_ptr<Context> &context, const pair<Token *, Token *> &location, const vector<shared_ptr<Object>> &other, const any &a1, const any &a2, const any &a3, bool createContext) const {
	auto ctx = context;
	if (createContext) {
		ctx = Context::create(context);
	}
	auto ptr = creator(ctx, a1, a2, a3);
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

Variable::Variable(const shared_ptr<Context> &context, string name, shared_ptr<Object> value, bool immutable)
		: Object(context, nullptr), name(move(name)), value(move(value)), immutable(immutable) {
}

void Variable::setValue(const shared_ptr<Object> &val) {
	if (!immutable)
		value = val->as<Object>();
	else throw compile_error("Cannot assign immutable variable '" + name + "'", {});
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

shared_ptr<Executable> Variable::findMethod(const string &methodName) const {
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

Fun::Fun(const shared_ptr<Context> &context, const vector<Param> &params, vector<shared_ptr<Callable>> calls)
		: Executable(context, FunClass), params(params), calls(move(calls)) {}

shared_ptr<Object> Fun::execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const pair<Token *, Token *> &location) {
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
			throw compile_error("Fun takes at least " + to_string(params.size() - 1)
								+ " arguments, but " + to_string(args.size()) + " were given", location);
	} else if (params.size() != args.size()) {
		throw compile_error("Fun takes " + to_string(params.size())
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
		returnValue = call->invoke();
	}
	return returnValue;
}

NativeFun::NativeFun(const shared_ptr<Context> &context, const vector<Param> &params, method_t code)
		: Fun(context, params, {}), code(move(code)) {}

shared_ptr<Object> NativeFun::execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const pair<Token *, Token *> &location) {
	if (self->as<Object>() == shared_from_this()) {
		if (!params.empty() && params.back().rest) {
			if (params.size() - 1 > other.size())
				throw compile_error("Fun takes at least " + to_string(params.size() - 1)
									+ " arguments, but " + to_string(other.size()) + " were given", location);
		} else {
			if (params.size() != other.size())
				throw compile_error("Fun takes " + to_string(params.size())
									+ " arguments, but " + to_string(other.size()) + " were given", location);
		}
	} else {
		if (!params.empty() && params.back().rest) {
			if (params.size() - 1 > other.size() + 1)
				throw compile_error("Fun takes at least " + to_string(params.size() - 1)
									+ " arguments, but " + to_string(other.size() + 1) + " were given", location);
		} else {
			if (params.size() != other.size() + 1)
				throw compile_error("Fun takes " + to_string(params.size())
									+ " arguments, but " + to_string(other.size() + 1) + " were given", location);
		}
	}
	auto returnValue = code(self, other, location);
	return returnValue;
}

shared_ptr<NativeFun> NativeFun::create(const vector<Param> &params, const NativeFun::method_t &code) {
	return make_shared<NativeFun>(Context::global, params, code);
}

Block::Block(const shared_ptr<Context> &context, vector<shared_ptr<Callable>> calls)
		: Executable(context, BlockClass), calls(move(calls)) {}

shared_ptr<Object> Block::execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const pair<Token *, Token *> &location) {
	shared_ptr<Object> returnValue = Void;
	for (auto &call: calls) {
		returnValue = call->invoke();
	}
	return returnValue;
}

shared_ptr<Object> Block::execute(const pair<Token *, Token *> &location) {
	return execute(shared_from_this(), {}, location);
}

Array::Array(const shared_ptr<Context> &context) : Object(context, ArrayClass) {}

void initializeContext() {
	Context::global = Context::create(nullptr);
	Context::global->set("import", NativeFun::create({{"moduleName"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const pair<Token *, Token *> &location) {
		auto arg = other[0];
		if (arg->getType() == String::TYPE) {
			auto moduleName = arg->as<String>()->value;
			try {
				const auto &pool = Pool::execute(moduleName);
				if (!pool.getResult()) {
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
	ClassClass = make_shared<Class>(Context::create(Context::global), Class::CREATOR, string(Class::TYPE), shared_ptr<Class>(nullptr))->as<Class>(); // ObjectClass is not yet created, so we pass nullptr as super
	ObjectClass = ClassClass->newInstance(Context::global, {}, {}, static_cast<Class::creator_t>(Object::CREATOR), string(Object::TYPE), shared_ptr<Class>(nullptr))->as<Class>(); // ObjectClass is the base class, so it has no super
	ClassClass->super = ObjectClass; // Now ObjectClass exists, so we can assign it to ClassClass->super
	BoolClass = ObjectClass->extend(Bool::CREATOR, string(Bool::TYPE), {}, {});
	IntegerClass = ObjectClass->extend(Integer::CREATOR, string(Integer::TYPE), {}, {});
	DecimalClass = ObjectClass->extend(Decimal::CREATOR, string(Decimal::TYPE), {}, {});
	StringClass = ObjectClass->extend(String::CREATOR, string(String::TYPE), {}, {});
	ArrayClass = ObjectClass->extend(Array::CREATOR, string(Array::TYPE), {}, {});
	BlockClass = ObjectClass->extend(Block::CREATOR, string(Block::TYPE), {}, {});
	FunClass = ObjectClass->extend(Fun::CREATOR, string(Fun::TYPE), {}, {});
	VoidClass = ObjectClass->extend(nullptr, "Void", {}, {});
	NothingClass = ObjectClass->extend(nullptr, "Nothing", {}, {});
	Void = VoidClass->newInstance(Context::global, {}, {}, VoidClass);
	Null = NothingClass->newInstance(Context::global, {}, {}, NothingClass);
	True = BoolClass->newInstance(Context::global, {}, {}, true)->as<Bool>();
	False = BoolClass->newInstance(Context::global, {}, {}, false)->as<Bool>();
}

void pool::initializeStdLib() {
	initializeContext();
	initializeBaseObjects();
}
