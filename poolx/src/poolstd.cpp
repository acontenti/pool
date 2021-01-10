#include <iostream>
#include "poolstd.hpp"
#include "poolx.hpp"

using namespace pool;

long Object::ID_COUNTER = 0;
long Class::INSTANCES = 0;
bool pool::debug = false;
vector<shared_ptr<Object>> pool::arguments{};
const shared_ptr<Context> Context::global = shared_ptr<Context>(new Context);
const shared_ptr<Class> pool::ClassClass = Class::create("Class", nullptr); //ObjectClass is null
const shared_ptr<Class> pool::ObjectClass = Class::create("Object", nullptr);
const shared_ptr<Class> pool::BoolClass = Class::create("Bool", ObjectClass);
const shared_ptr<Class> pool::IntegerClass = Class::create("Integer", ObjectClass);
const shared_ptr<Class> pool::DecimalClass = Class::create("Decimal", ObjectClass);
const shared_ptr<Class> pool::StringClass = Class::create("String", ObjectClass);
const shared_ptr<Class> pool::TupleClass = Class::create("Tuple", ObjectClass);
const shared_ptr<Class> pool::ArrayClass = Class::create("Array", ObjectClass);
const shared_ptr<Class> pool::CallClass = Class::create("Call", ObjectClass);
const shared_ptr<Class> pool::IdentityClass = Class::create("Identity", ObjectClass);
const shared_ptr<Class> pool::VariableClass = Class::create("Variable", ObjectClass);
const shared_ptr<Class> pool::BlockClass = Class::create("Block", ObjectClass);
const shared_ptr<Class> pool::VoidClass = Class::create("Void", ObjectClass);
const shared_ptr<Class> pool::NothingClass = Class::create("Nothing", ObjectClass);
const shared_ptr<Object> pool::Void = Object::create(VoidClass, Context::global);
const shared_ptr<Object> pool::Null = Object::create(NothingClass, Context::global);

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
			var->setValue(value);
		} else {
			add(Variable::create(name, value, shared_from_this(), false));
		}
	}
}

shared_ptr<Variable> Context::add(const shared_ptr<Variable> &var) {
	return heap.try_emplace(var->name, var).first->second;
}

string_view Object::getType() const {
	return cls->name;
}

const Object::method_t *Object::findMethod(const string &methodName) const {
	return cls->getMethod(methodName);
}

shared_ptr<Object> Object::getVariableValue() {
	return reinterpret_pointer_cast<Variable>(shared_from_this())->getValue();
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
		: Object(Context::create(context), ClassClass), name(move(name)), super(move(super)) {}

const Object::method_t *Class::findMethod(const string &methodName) const {
	auto result = Object::findMethod(methodName);
	if (result) {
		return result;
	} else {
		auto siterator = staticMethodsMap.find(methodName);
		if (siterator != staticMethodsMap.end()) {
			return &siterator->second;
		} else return nullptr;
	}
}

vector<shared_ptr<Object>> Block::makeArgs(const shared_ptr<Object> &other, const shared_ptr<Object> &prepend) {
	vector<shared_ptr<Object>> args;
	if (prepend)
		args.push_back(prepend);
	if (other->getType() == "Void");
	else if (other->getType() == "Tuple") {
		args = other->as<Tuple>()->values;
	} else {
		args.push_back(other);
	}
	return args;
}

bool pool::initialize() noexcept try {
	ClassClass->super = ObjectClass;
	ClassClass->addMethod("extend", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		if (other->getType() == "Block") {
			other->as<Block>()->execute({});
			auto cls = Class::create("Class" + to_string(Class::INSTANCES++), self->as<Class>(), self->context);
			for (auto[name, var] : *other->context) {
				if (var->getType() == "Block") {
					auto block = var->as<Block>();
					if (!block->params.empty()) {
						if (block->params[0] == "this") {
							cls->addMethod(name, [block](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
								return block->execute(Block::makeArgs(other, self->as<Object>()));
							});
						} else if (block->params[0] == "class") {
							cls->addStaticMethod(name, [block](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
								return block->execute(Block::makeArgs(other, self->as<Object>()));
							});
						} else {
							cls->context->add(var);
						}
					} else {
						cls->context->add(var);
					}
				} else {
					cls->context->add(var);
				}
			}
			return cls;
		} else return Null;
	});
	ClassClass->addMethod("new", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		auto cls = self->as<Class>();
		auto ptr = Object::create(cls, Context::create(self->context));
		auto init = cls->getMethod("init");
		if (init) {
			(*init)(ptr, other);
		}
		return ptr;
	});
	ObjectClass->addMethod(".", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		if (other->isVariable()) {
			auto id = other->as<Variable>()->name;
			auto value = self->as<Object>();
			auto ctx = value->context;
			const shared_ptr<Object> &result = ctx->find(id);
			return result ? result : ctx->add(Variable::create(id, ctx));
		} else throw execution_error("Invalid value for . call");
	});
	ObjectClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) {
		return String::create(self->toString(), self->context);
	});
	Context::global->add(Variable::create("Object", ObjectClass, Context::global, true));
	VoidClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return String::create("void", self->context);
	});
	NothingClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return String::create("null", self->context);
	});
	BoolClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return String::create(self->as<Bool>()->value ? "true" : "false", self->context);
	});
	BoolClass->addMethod("!", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return Bool::create(!self->as<Bool>()->value, self->context);
	});
	Context::global->add(Variable::create("Conditional", Context::global));
	BoolClass->addMethod("then", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		if (other->getType() == "Block") {
			bool condition = self->as<Bool>()->value;
			if (condition) {
				other->as<Block>()->execute({});
			}
			auto conditional = self->context->find("Conditional");
			if (conditional)
				if (auto newF = conditional->findMethod("new"))
					return (*newF)(conditional, Bool::create(condition, self->context));
			throw execution_error("Method new not found");
		} else return Null;
	});
	IntegerClass->addMethod("+", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		auto value = self->as<Integer>()->value;
		if (other->getType() == "Integer") {
			return Integer::create(value + other->as<Integer>()->value, self->context);
		} else if (other->getType() == "Decimal") {
			return Decimal::create(value + other->as<Decimal>()->value, self->context);
		} else if (other->getType() == "String") {
			return String::create(to_string(value) + other->as<String>()->value, self->context);
		} else return Null;
	});
	IntegerClass->addMethod("-", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		auto value = self->as<Integer>()->value;
		if (other->getType() == "Integer") {
			return Integer::create(value - other->as<Integer>()->value, self->context);
		} else if (other->getType() == "Decimal") {
			return Decimal::create(value - other->as<Decimal>()->value, self->context);
		} else return Null;
	});
	IntegerClass->addMethod("++", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		self->as<Integer>()->value++;
		return self;
	});
	IntegerClass->addMethod("--", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		self->as<Integer>()->value--;
		return self;
	});
	IntegerClass->addMethod("+=", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		auto ptr = self->as<Integer>();
		if (other->getType() == "Integer") {
			ptr->value += other->as<Integer>()->value;
		} else if (other->getType() == "Decimal") {
			ptr->value += other->as<Decimal>()->value;
		} else return Null;
		return self;
	});
	IntegerClass->addMethod("-=", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		auto ptr = self->as<Integer>();
		if (other->getType() == "Integer") {
			ptr->value -= other->as<Integer>()->value;
		} else if (other->getType() == "Decimal") {
			ptr->value -= other->as<Decimal>()->value;
		} else return Null;
		return self;
	});
	IntegerClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return String::create(to_string(self->as<Integer>()->value), self->context);
	});
	DecimalClass->addMethod("+", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		auto value = self->as<Decimal>()->value;
		if (other->getType() == "Integer") {
			return Decimal::create(value + other->as<Integer>()->value, self->context);
		} else if (other->getType() == "Decimal") {
			return Decimal::create(value + other->as<Decimal>()->value, self->context);
		} else if (other->getType() == "String") {
			return String::create(to_string(value) + other->as<String>()->value, self->context);
		} else return Null;
	});
	DecimalClass->addMethod("-", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		auto value = self->as<Decimal>()->value;
		if (other->getType() == "Integer") {
			return Decimal::create(value - other->as<Integer>()->value, self->context);
		} else if (other->getType() == "Decimal") {
			return Decimal::create(value - other->as<Decimal>()->value, self->context);
		} else return Null;
	});
	DecimalClass->addMethod("+=", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		auto ptr = self->as<Decimal>();
		if (other->getType() == "Integer") {
			ptr->value += other->as<Integer>()->value;
		} else if (other->getType() == "Decimal") {
			ptr->value += other->as<Decimal>()->value;
		} else return Null;
		return self;
	});
	DecimalClass->addMethod("-=", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		auto ptr = self->as<Decimal>();
		if (other->getType() == "Integer") {
			ptr->value -= other->as<Integer>()->value;
		} else if (other->getType() == "Decimal") {
			ptr->value -= other->as<Decimal>()->value;
		} else return Null;
		return self;
	});
	DecimalClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return String::create(to_string(self->as<Decimal>()->value), self->context);
	});
	StringClass->addMethod("+", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		shared_ptr<Object> var = other->as<Object>();
		if (auto method = var->findMethod("toString")) {
			auto result = (*method)(var, Void);
			if (result->getType() == "String") {
				return String::create(self->as<String>()->value + result->as<String>()->value, self->context);
			}
		}
		return Null;
	});
	StringClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return self;
	});
	StringClass->addMethod("import", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		auto moduleName = self->as<String>()->value;
		try {
			PoolX::execute(moduleName);
		} catch (const compile_error &e) {
			throw PoolX::compile_fatal("Cannot import module \"" + moduleName + "\": " + e.message());
		}
		return Void;
	});
	VariableClass->addMethod("=", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		self->as<Variable>()->setValue(other->as<Object>());
		return self;
	});
	VariableClass->addMethod(":=", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		self->as<Variable>()->setValue(other->as<Object>());
		self->as<Variable>()->setImmutable(true);
		return self;
	});
	BlockClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		stringstream ss;
		auto ctx = self->context;
		ss << "{";
		if (!ctx->empty()) {
			for (auto &item : *ctx) {
				shared_ptr<Object> var = item.second->getValue()->as<Object>();
				if (auto method = var->findMethod("toString")) {
					auto result = (*method)(var, Void);
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
		}
		ss << "}";
		return String::create(ss.str(), ctx);
	});
	BlockClass->addMethod("->", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return self->as<Block>()->execute(Block::makeArgs(other));
	});
	auto IoClass = Class::create("Io", ObjectClass);
	IoClass->addMethod("println", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		shared_ptr<Object> var = other->as<Object>();
		if (auto method = var->findMethod("toString")) {
			auto result = (*method)(var, Void);
			if (result->getType() == "String") {
				cout << result->as<String>()->value << endl;
			}
		}
		return Void;
	});
	auto Io = Object::create(IoClass, Context::global);
	Context::global->add(Variable::create("stdout", Io, Context::global, true));
	return true;
} catch (...) {
	cerr << "Initialization error" << endl;
	return false;
}

[[maybe_unused]]
const bool pool::initialized = initialize();