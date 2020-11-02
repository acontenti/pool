#include <iostream>
#include "poolstd.hpp"

using namespace pool;

long Object::ID_COUNTER = 0;
const shared_ptr<Context> Context::global = shared_ptr<Context>(new Context);
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
const shared_ptr<Tuple> Tuple::Empty = make_shared<Tuple>(vector<shared_ptr<Object>>(), Context::global);
const shared_ptr<Block> Block::Empty = make_shared<Block>(vector<string>(), vector<shared_ptr<Call>>(), Context::global);
const shared_ptr<Call> Call::Empty = make_shared<Call>(Block::Empty, "->", Void, false, Context::global);

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
			add(make_shared<Variable>(name, value, shared_from_this()));
		}
	}
}

void Context::add(const shared_ptr<Variable> &var) {
	heap.emplace(var->name, var);
}

string_view Object::getType() const {
	return cls->name;
}

const Object::method_t *Object::findMethod(const string &methodName) const {
	return cls->getMethod(methodName);
}

shared_ptr<Object> Object::getVariableValue() {
	return reinterpret_pointer_cast<Variable>(shared_from_this())->value;
}

const Object::method_t *Class::getMethod(const string &methodName) const {
	auto iterator = methodsMap.find(methodName);
	if (iterator != methodsMap.end()) {
		return &iterator->second;
	} else if (super) {
		return super->getMethod(methodName);
	} else return nullptr;
}

Class::Class(string name, shared_ptr<Class> super, const shared_ptr<Context> &context)
		: Object(context, ClassClass ? ClassClass : shared_ptr<Class>(this)), name(move(name)), super(move(super)) {}

bool pool::initialize() noexcept try {
	ClassClass->super = ObjectClass;
	ClassClass->addMethod("extend", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		if (other->getType() == "String") {
			return make_shared<Class>(other->as<String>()->value, self->as<Class>(), self->context);
		} else return Null;
	});
	ClassClass->addMethod("new", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		auto cls = self->as<Class>();
		auto ptr = make_shared<Object>(self->context, cls);
		vector<shared_ptr<Object>> args;
		if (other->getType() == "Void");
		else if (other->getType() == "Tuple") {
			args = other->as<Tuple>()->values;
		} else {
			args.push_back(other);
		}
		auto init = cls->context->find("init");
		if (init && init->getType() == "Block") {
			init->as<Block>()->execute(args);
		}
		return ptr;
	});
	ObjectClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) {
		return make_shared<String>(self->toString(), self->context);
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
	BoolClass->addMethod("!", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return make_shared<Bool>(!self->as<Bool>()->value, self->context);
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
	IntegerClass->addMethod("++", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		self->as<Integer>()->value++;
		return self;
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
			auto ctx = value->as<Object>()->context;
			const shared_ptr<Object> &result = ctx->find(id);
			return result ? result : make_shared<Variable>(id, ctx);
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
		if (other->getType() == "Void");
		else if (other->getType() == "Tuple") {
			args = other->as<Tuple>()->values;
		} else {
			args.push_back(other);
		}
		return self->as<Block>()->execute(args);
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
	Context::global->add(make_shared<Variable>("stdout", Io, Context::global));
	Context::global->add(make_shared<Variable>("Object", ObjectClass, Context::global));
	return true;
} catch (...) {
	cerr << "Initialization error" << endl;
	return false;
}

[[maybe_unused]]
const bool pool::initialized = initialize();