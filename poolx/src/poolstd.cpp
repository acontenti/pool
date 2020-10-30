#include <iostream>
#include "poolstd.hpp"

using namespace pool;

long Object::ID_COUNTER = 0;
const shared_ptr<Context> Context::global = shared_ptr<Context>(new Context);
const shared_ptr<Tuple> Tuple::Empty = make_shared<Tuple>(vector<shared_ptr<Object>>(), Context::global);
const shared_ptr<Block> Block::Empty = make_shared<Block>(vector<string>(), vector<shared_ptr<Call>>(), Context::global);
const shared_ptr<Call> Call::Empty = make_shared<Call>(Block::Empty, "->", Void, Context::global);
const shared_ptr<Class> pool::ClassClass = make_shared<Class>("Class", nullptr); //ObjectClass is null
const shared_ptr<Class> pool::ObjectClass = make_shared<Class>("Object", nullptr);
const shared_ptr<Class> pool::BoolClass = make_shared<Class>("Bool", ObjectClass);
const shared_ptr<Class> pool::IntegerClass = make_shared<Class>("Integer", ObjectClass);
const shared_ptr<Class> pool::DecimalClass = make_shared<Class>("Decimal", ObjectClass);
const shared_ptr<Class> pool::StringClass = make_shared<Class>("String", ObjectClass);
const shared_ptr<Class> pool::TupleClass = make_shared<Class>("Tuple", ObjectClass);
const shared_ptr<Class> pool::ArrayClass = make_shared<Class>("Array", ObjectClass);
const shared_ptr<Class> pool::CallClass = make_shared<Class>("Call", ObjectClass);
const shared_ptr<Class> pool::IdentityClass = make_shared<Class>("Identity", ObjectClass);
const shared_ptr<Class> pool::VariableClass = make_shared<Class>("Variable", ObjectClass);
const shared_ptr<Class> pool::BlockClass = make_shared<Class>("Block", ObjectClass);
const shared_ptr<Class> pool::VoidClass = make_shared<Class>("Void", ObjectClass);
const shared_ptr<Class> pool::NothingClass = make_shared<Class>("Nothing", ObjectClass);
const shared_ptr<Object> pool::Void = make_shared<Object>(Context::global, VoidClass);
const shared_ptr<Object> pool::Null = make_shared<Object>(Context::global, NothingClass);

template<>
shared_ptr<Variable> Object::as() {
	return reinterpret_pointer_cast<Variable>(shared_from_this());
}

shared_ptr<Variable> Context::find(const string &name) const {
	auto iterator = heap.find(name);
	if (iterator != heap.end()) {
		return iterator->second;
	} else if (parent) {
		return parent->find(name);
	} else return nullptr;
}

void Context::associate(const vector<string> &params, const vector<shared_ptr<Object>> &args) {
	for (int i = 0; i < params.size(); ++i) {
		auto name = params[i];
		auto value = args[i];
		if (const auto &var = find(name)) {
			var->value = value;
		} else {
			add(name, make_shared<Variable>(name, value, shared_from_this()));
		}
	}
}

string_view Object::getType() const {
	return cls->name;
}

const Object::method_t *Object::findMethod(const string &methodName) const {
	return cls->findMethod(methodName);
}

shared_ptr<Object> Object::getVariableValue() {
	return reinterpret_pointer_cast<Variable>(shared_from_this())->value;
}

const Object::method_t *Class::findMethod(const string &methodName) const {
	auto iterator = methodsMap.find(methodName);
	if (iterator != methodsMap.end()) {
		return &iterator->second;
	} else if (super) {
		return super->findMethod(methodName);
	} else return nullptr;
}

bool pool::initialize() noexcept try {
	ClassClass->super = ObjectClass;
	ObjectClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) {
		return make_shared<String>(self->toString(), self->context);
	});
	ObjectClass->addMethod("extend", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		if (other->getType() == "String") {
			return make_shared<Class>(other->as<String>()->value, self->as<Class>(), self->context);
		} else return Null;
	});
	ObjectClass->addMethod("new", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return make_shared<Object>(self->context, self->as<Class>());
	});
	VoidClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return make_shared<String>("void", self->context);
	});
	NothingClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return make_shared<String>("null", self->context);
	});
	BoolClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return make_shared<String>(self->as<Bool>()->value ? "true" : "false", self->context);
	});
	IntegerClass->addMethod("+", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		auto value = self->as<Integer>()->value;
		if (other->getType() == "Integer") {
			return make_shared<Integer>(value + other->as<Integer>()->value, self->context);
		} else if (other->getType() == "Decimal") {
			return make_shared<Decimal>(value + other->as<Decimal>()->value, self->context);
		} else if (other->getType() == "String") {
			return make_shared<String>(to_string(value) + other->as<String>()->value, self->context);
		} else return Null;
	});
	IntegerClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return make_shared<String>(to_string(self->as<Integer>()->value), self->context);
	});
	DecimalClass->addMethod("+", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		auto value = self->as<Decimal>()->value;
		if (other->getType() == "Integer") {
			return make_shared<Decimal>(value + other->as<Integer>()->value, self->context);
		} else if (other->getType() == "Decimal") {
			return make_shared<Decimal>(value + other->as<Decimal>()->value, self->context);
		} else if (other->getType() == "String") {
			return make_shared<String>(to_string(value) + other->as<String>()->value, self->context);
		} else return Null;
	});
	DecimalClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return make_shared<String>(to_string(self->as<Decimal>()->value), self->context);
	});
	StringClass->addMethod("+", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		shared_ptr<Object> var = other->as<Object>();
		if (auto method = var->findMethod("toString")) {
			auto result = (*method)(var, Tuple::Empty);
			if (result->getType() == "String") {
				return make_shared<String>(self->as<String>()->value + result->as<String>()->value, self->context);
			}
		}
		return Null;
	});
	StringClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return self->shared_from_this();
	});
	IdentityClass->addMethod("", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return self->as<struct Call::Identity>()->result;
	});
	VariableClass->addMethod("=", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		self->as<Variable>()->value = other->as<Object>();
		return self->shared_from_this();
	});
	VariableClass->addMethod(".", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		if (other->isVariable()) {
			auto id = other->as<Variable>()->name;
			auto &value = self->as<Variable>()->value;
			if (value->getType() == "Block") {
				const shared_ptr<Object> &result = value->as<Block>()->context->find(id);
				return result ? result : Null;
			} else return Null;
		} else throw execution_error(__FILE__, __LINE__, "Invalid value for . call");
	});
	BlockClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		stringstream ss;
		ss << "{";
		for (auto &item : *self->context) {
			shared_ptr<Object> var = item.second->value->as<Object>();
			if (auto method = var->findMethod("toString")) {
				auto result = (*method)(var, Tuple::Empty);
				ss << item.first << ":";
				if (result->getType() == "String") {
					ss << result->as<String>()->value;
				} else {
					ss << result;
				}
				ss << ",";
			}
		}
		ss.seekp(-1, stringstream::cur); //remove last comma
		ss << "}";
		return make_shared<String>(ss.str(), self->context);
	});
	BlockClass->addMethod("->", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		vector<shared_ptr<Object>> args;
		if (other->getType() == "Empty");
		else if (other->getType() == "Tuple") {
			args = other->as<Tuple>()->values;
		} else {
			args.push_back(other);
		}
		if (self->as<Block>()->params.size() == args.size()) {
			return self->as<Block>()->execute(args);
		} else return Null;
	});
	auto IoClass = make_shared<Class>("Io", ObjectClass);
	IoClass->addMethod("println", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		shared_ptr<Object> var = other->as<Object>();
		if (auto method = var->findMethod("toString")) {
			auto result = (*method)(var, Tuple::Empty);
			if (result->getType() == "String") {
				cout << result->as<String>()->value << endl;
			}
		}
		return Void;
	});
	const shared_ptr<Object> &Io = make_shared<Object>(Context::global, IoClass);
	Context::global->add("stdout", make_shared<Variable>("stdout", Io, Context::global));
	Context::global->add("Object", make_shared<Variable>("Object", ObjectClass, Context::global));
	return true;
} catch (...) {
	cerr << "Initialization error" << endl;
	return false;
}

[[maybe_unused]]
const bool pool::initialized = initialize();