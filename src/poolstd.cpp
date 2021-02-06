#include "poolstd.hpp"

using namespace pool;

size_t Class::COUNTER = 0;
bool pool::debug = false;
vector<shared_ptr<Object>> pool::arguments{};

Object::Object(shared_ptr<Class> cls, const shared_ptr<Context> &context)
		: context(context), cls(move(cls)), id(cls ? cls->instances++ : 0) {
	if (this->cls) {
		this->context->add("class", this->cls);
	}
}

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

shared_ptr<Object> Object::find(const string &name) const {
	if (auto local = context->find(name)) {
		return local;
	}
	if (cls) {
		return cls->findInClass(name);
	}
	return nullptr;
}

shared_ptr<Object> Object::findLocal(const string &name) const {
	return context->findLocal(name);
}

shared_ptr<Object> Object::getVariableValue() {
	return reinterpret_pointer_cast<Variable>(shared_from_this())->getValue();
}

template<>
shared_ptr<Variable> Object::as<Variable>() {
	return reinterpret_pointer_cast<Variable>(shared_from_this());
}

Class::Class(const shared_ptr<Context> &context, string name, shared_ptr<Class> super)
		: Object(ClassClass, Context::create(context)), name(move(name)), super(move(super)) {}

shared_ptr<Class> Class::extend(const string &className, const shared_ptr<Block> &other) {
	auto self = this->as<Class>();
	auto cls = ClassClass->newInstance<Class>(context, {}, className, self);
	cls->context->set("super", self);
	if (other) {
		other->invoke();
		for (auto[name, value] : *other->context) {
			cls->context->add(name, value->as<Object>());
		}
	}
	return cls;
}

void Class::callInit(const shared_ptr<Object> &ptr, const vector<shared_ptr<Object>> &other) {
	if (auto init = ptr->findMethod("init")) {
		init->execute(ptr, other);
	}
}

template<>
shared_ptr<Object> Class::newInstance<Object>(const shared_ptr<Context> &context, const vector<shared_ptr<Object>> &other) {
	auto ptr = make_shared<Object>(this->as<Class>(), context);
	callInit(ptr, other);
	return ptr;
}

shared_ptr<Object> Array::invoke() {
	if (values.size() != calls.size()) {
		values.reserve(calls.size());
		for (auto &call : calls) {
			values.emplace_back(call->invoke());
		}
	}
	return shared_from_this();
}

shared_ptr<Object> Array::execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) {
	if (other.size() == params.size()) {
		auto arg = other[0];
		if (arg->getType() == Integer::TYPE) {
			auto index = arg->as<Integer>()->value;
			if (index >= 0 && index < calls.size()) {
				invoke();
				return values[index];
			} else throw execution_error("Array access index out of range");
		} else throw execution_error("Array access argument must be an integer");
	} else throw execution_error("Array access needs an argument");
}
