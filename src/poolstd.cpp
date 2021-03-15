#include "poolstd.hpp"

using namespace pool;

size_t Class::COUNTER = 0;
bool pool::debug = false;
vector<shared_ptr<Object>> pool::arguments{};

Object::Object(const shared_ptr<Context> &context, shared_ptr<Class> cls)
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

Class::Class(const shared_ptr<Context> &context, creator_t creator, string name, shared_ptr<Class> super)
		: Object(Context::create(context), ClassClass), creator(creator), name(move(name)), super(move(super)) {}

shared_ptr<Class> Class::extend(creator_t _creator, const string &className, const shared_ptr<Block> &other) {
	auto self = this->as<Class>();
	auto cls = ClassClass->newInstance(context, {}, _creator ? _creator : creator, className, self)->as<Class>();
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
