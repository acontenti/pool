#include "poolstd.hpp"

using namespace pool;

bool pool::debug = false;
vector<shared_ptr<Object>> pool::arguments{};

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

shared_ptr<Object> Object::getVariableValue() {
	return reinterpret_pointer_cast<Variable>(shared_from_this())->getValue();
}

template<>
shared_ptr<Variable> Object::as<Variable>() {
	return reinterpret_pointer_cast<Variable>(shared_from_this());
}

Class::Class(const shared_ptr<Context> &context, creator_t creator, string name, shared_ptr<Class> super)
		: Object(context, ClassClass), creator(move(creator)), name(move(name)), super(move(super)) {}

shared_ptr<Class> Class::extend(const creator_t &_creator, const string &className, const shared_ptr<Block> &other) {
	auto self = this->as<Class>();
	auto cls = ClassClass->newInstance(context, {}, _creator ? _creator : creator, className, self)->as<Class>();
	cls->context->set("super", self);
	if (other) {
		other->invoke();
		for (auto[name, value] : *other->context) {
			cls->context->set(name, value->as<Object>());
		}
	}
	return cls;
}

shared_ptr<Object> Class::newInstance(const shared_ptr<Context> &context, const vector<shared_ptr<Object>> &other, const any &a1, const any &a2, const any &a3, bool createContext) const {
	auto ctx = context;
	if (createContext) {
		ctx = Context::create(context);
	}
	auto ptr = creator(ctx, a1, a2, a3);
	if (ptr->cls) {
		ptr->context->set("class", ptr->cls);
	}
	if (auto init = ptr->findMethod("init")) {
		init->execute(ptr, other);
	}
	return ptr;
}

shared_ptr<Object> NativeFun::execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) {
	if (self->as<Object>() == shared_from_this()) {
		if (params.back().rest) {
			if (params.size() - 1 > other.size())
				throw execution_error("Parameters number does not match arguments number");
		} else {
			if (params.size() != other.size())
				throw execution_error("Parameters number does not match arguments number");
		}
	} else {
		if (params.back().rest) {
			if (params.size() - 1 > other.size() + 1)
				throw execution_error("Parameters number does not match arguments number");
		} else {
			if (params.size() != other.size() + 1)
				throw execution_error("Parameters number does not match arguments number");
		}
	}
	auto returnValue = code(self, other);
	return returnValue;
}

shared_ptr<Object> Fun::execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) {
	shared_ptr<Object> returnValue = Void;
	vector<shared_ptr<Object>> args;
	if (self) {
		auto ptr = self->as<Object>();
		if (ptr != shared_from_this()) {
			args.reserve(other.size() + 1);
			args.push_back(ptr);
		}
	} else {
		args.reserve(other.size());
	}
	args.insert(args.end(), other.begin(), other.end());
	if (!params.empty() && params.back().rest) {
		if (params.size() - 1 > args.size())
			throw execution_error("Parameters number does not match arguments number");
	} else {
		if (params.size() != args.size())
			throw execution_error("Parameters number does not match arguments number");
	}
	associateContext(args);
	for (auto &call: calls) {
		returnValue = call->invoke();
	}
	return returnValue;
}

void Fun::associateContext(const vector<shared_ptr<pool::Object>> &args) {
	for (int i = 0; i < params.size(); ++i) {
		shared_ptr<Object> value;
		auto name = params[i].id;
		if (params[i].rest) {
			vector<shared_ptr<Object>> rest;
			rest.reserve(args.size() - i);
			for (int j = i; j < args.size(); ++j) {
				rest.emplace_back(args[j]);
			}
			value = ArrayClass->newInstance(context, rest);
		} else {
			value = args[i];
		}
		context->set(name, value);
	}
}

shared_ptr<Object> Block::invoke() {
	shared_ptr<Object> returnValue = Void;
	for (auto &call: calls) {
		returnValue = call->invoke();
	}
	return returnValue;
}
