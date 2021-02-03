#include <iostream>
#include "poolstd.hpp"
#include "pool.hpp"

using namespace pool;

size_t Class::COUNTER = 0;
bool pool::debug = false;
vector<shared_ptr<Object>> pool::arguments{};
const shared_ptr<Context> Context::global = shared_ptr<Context>(new Context);
const shared_ptr<Class> pool::ClassClass = Class::create(string(Class::TYPE), nullptr); //ObjectClass is null
const shared_ptr<Class> pool::ObjectClass = Class::create("Object", nullptr);
const shared_ptr<Class> pool::BoolClass = Class::create(string(Bool::TYPE), ObjectClass);
const shared_ptr<Class> pool::IntegerClass = Class::create(string(Integer::TYPE), ObjectClass);
const shared_ptr<Class> pool::DecimalClass = Class::create(string(Decimal::TYPE), ObjectClass);
const shared_ptr<Class> pool::StringClass = Class::create(string(String::TYPE), ObjectClass);
const shared_ptr<Class> pool::ArrayClass = Class::create("Array", ObjectClass);
const shared_ptr<Class> pool::VariableClass = Class::create("Variable", ObjectClass);
const shared_ptr<Class> pool::BlockClass = Class::create("Block", ObjectClass);
const shared_ptr<Class> pool::FunClass = Class::create("Function", ObjectClass);
const shared_ptr<Class> pool::VoidClass = Class::create("Void", ObjectClass);
const shared_ptr<Class> pool::NothingClass = Class::create("Nothing", ObjectClass);
const shared_ptr<Object> pool::Void = Object::create(VoidClass, Context::global, {});
const shared_ptr<Object> pool::Null = Object::create(NothingClass, Context::global, {});

template<>
shared_ptr<Variable> Object::as() {
	return reinterpret_pointer_cast<Variable>(shared_from_this());
}

shared_ptr<Object> Context::find(const string &name) const {
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
			if (var->isVariable())
				var->as<Variable>()->setValue(value);
			else {
				set(name, value);
			}
		} else {
			add(name, value);
		}
	}
}

shared_ptr<Object> Context::add(const string &name, const shared_ptr<Object> &var) {
	return heap.try_emplace(name, var).first->second;
}

shared_ptr<Object> Context::add(const string &name) {
	return heap.try_emplace(name, Variable::create(name, shared_from_this())).first->second;
}

void Context::set(const string &name, const shared_ptr<Object> &value) {
	heap[name] = value;
}

shared_ptr<Executable> Context::findMethod(const string &name) const {
	if (auto object = find(name)) {
		if (auto executable = dynamic_pointer_cast<Executable>(object->as<Object>())) {
			return executable;
		}
	}
	return nullptr;
}

string Context::toString() const {
	stringstream ss;
	ss << "{";
	if (!heap.empty()) {
		for (auto&[name, value] : *this) {
			ss << name << ":" << value->toString() << ",";
		}
		ss.seekp(-1, stringstream::cur); //remove last comma
	}
	ss << "}";
	return ss.str();
}

Object::Object(shared_ptr<Context> context, shared_ptr<Class> cls)
		: context(move(context)), cls(move(cls)), id(cls ? cls->instances++ : 0) {
	if (this->cls) {
		this->context->add("class", this->cls);
	}
}

string_view Object::getType() {
	return cls ? cls->name : as<Class>()->name;
}

shared_ptr<Executable> Object::findMethod(const string &methodName) const {
	if (cls) {
		return cls->getMethod(methodName);
	} else return nullptr;
}

shared_ptr<Object> Object::find(const string &name) const {
	if (auto local = context->find(name)) {
		return local;
	}
	if (cls) {
		return cls->findLocal(name);
	}
	return nullptr;
}

shared_ptr<Object> Object::getVariableValue() {
	return reinterpret_pointer_cast<Variable>(shared_from_this())->getValue();
}

shared_ptr<Object> Object::create(const shared_ptr<Class> &cls, const shared_ptr<Context> &context, const vector<shared_ptr<Object>> &other) {
	auto ptr = make_shared<Object>(Context::create(context), cls);
	if (auto init = ptr->findMethod("init")) {
		init->execute(ptr, other);
	}
	return ptr;
}

Class::Class(string name, shared_ptr<Class> super, const shared_ptr<Context> &context)
		: Object(Context::create(context), ClassClass), name(move(name)), super(move(super)) {}

shared_ptr<Executable> Class::getMethod(const string &methodName) const {
	if (auto method = context->findMethod(methodName)) {
		return method;
	} else if (super) {
		return super->getMethod(methodName);
	} else return nullptr;
}

shared_ptr<Executable> Class::findMethod(const string &methodName) const {
	if (auto executable = Object::findMethod(methodName)) {
		return executable;
	} else if (auto method = getMethod(methodName)) {
		return method;
	} else return nullptr;
}

void Class::addMethod(const string &methodName, const method_t &method) {
	addMethod(methodName, NativeFun::create(method));
}

void Class::addMethod(const string &methodName, const shared_ptr<Executable> &method) {
	context->add(methodName, method);
}

shared_ptr<Object> Class::newInstance(const vector<shared_ptr<Object>> &other) {
	return Object::create(this->as<Class>(), context, other);
}

shared_ptr<Class> Class::extend(const shared_ptr<Block> &other) {
	other->invoke();
	auto className = string(Class::TYPE) + to_string(Class::COUNTER++);
	auto cls = Class::create(className, shared_from_this()->as<Class>(), context);
	for (auto[name, value] : *other->context) {
		cls->context->add(name, value->as<Object>());
	}
	return cls;
}

shared_ptr<Object> Invocation::invoke() {
	auto ptr = caller->invoke()->as<Object>();
	if (auto executable = dynamic_pointer_cast<Executable>(ptr)) {
		vector<shared_ptr<Object>> args(callee.size());
		transform(callee.begin(), callee.end(), args.begin(), [](const shared_ptr<Callable> &callable) {
			return callable->invoke();
		});
		return executable->execute(self->invoke(), args);
	} else throw execution_error(ptr->toString() + " is not executable");
}

shared_ptr<Object> Array::invoke() {
	values.reserve(calls.size());
	for (auto &call : calls) {
		values.emplace_back(call->invoke());
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

bool pool::initialize() noexcept try {
	ClassClass->super = ObjectClass;
	ClassClass->addMethod("extend", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		if (other.size() == 1 && other[0]->getType() == "Block") {
			return self->as<Class>()->extend(other[0]->as<Block>());
		} else throw execution_error("Class extend argument must be a block");
	});
	ClassClass->addMethod("new", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto cls = self->as<Class>();
		return cls->newInstance(other);
	});
	ObjectClass->addMethod("toString", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) {
		return String::create(self->toString(), self->context);
	});
	ObjectClass->addMethod("type", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) {
		return String::create(self->as<Object>()->context->toString(), self->context);
	});
	ObjectClass->addMethod("println", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		shared_ptr<Object> var = self->as<Object>();
		if (auto method = var->findMethod("toString")) {
			auto result = method->execute(var, {});
			if (result->getType() == String::TYPE) {
				cout << result->as<String>()->value << endl;
			}
		}
		return Void;
	});
	Context::global->add("Object", ObjectClass);
	VoidClass->addMethod("toString", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return String::create("void", self->context);
	});
	NothingClass->addMethod("toString", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return String::create("null", self->context);
	});
	BoolClass->addMethod("toString", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return String::create(self->as<Bool>()->value ? "true" : "false", self->context);
	});
	BoolClass->addMethod("!", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return Bool::create(!self->as<Bool>()->value, self->context);
	});
	Context::global->add("Conditional");
//	BoolClass->addMethod("then", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
//		if (other.size() == 1 && other[0]->getType() == "Block") {
//			bool condition = self->as<Bool>()->value;
//			if (condition) {
//				other[0]->as<Block>()->invoke();
//			}
//			auto conditional = self->context->find("Conditional");
//			if (conditional)
//				if (auto newF = conditional->findMethod("->")) {
//					return newF->execute(conditional, {Bool::create(condition, self->context)});
//				}
//			throw execution_error("Method -> not found");
//		} else return Null;
//	});
	IntegerClass->addMethod("+", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto value = self->as<Integer>()->value;
		if (other.size() != 1)
			return Null;
		auto par = other[0];
		if (par->getType() == Integer::TYPE) {
			return Integer::create(value + par->as<Integer>()->value, self->context);
		} else if (par->getType() == Decimal::TYPE) {
			return Decimal::create(value + par->as<Decimal>()->value, self->context);
		} else if (par->getType() == String::TYPE) {
			return String::create(to_string(value) + par->as<String>()->value, self->context);
		} else return Null;
	});
	IntegerClass->addMethod("-", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto value = self->as<Integer>()->value;
		if (other.size() != 1)
			return Null;
		auto par = other[0];
		if (par->getType() == Integer::TYPE) {
			return Integer::create(value - par->as<Integer>()->value, self->context);
		} else if (par->getType() == Decimal::TYPE) {
			return Decimal::create(value - par->as<Decimal>()->value, self->context);
		} else return Null;
	});
	IntegerClass->addMethod("++", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		self->as<Integer>()->value++;
		return self;
	});
	IntegerClass->addMethod("--", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		self->as<Integer>()->value--;
		return self;
	});
	IntegerClass->addMethod("+=", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto ptr = self->as<Integer>();
		if (other.size() != 1)
			return Null;
		auto par = other[0];
		if (par->getType() == Integer::TYPE) {
			ptr->value += par->as<Integer>()->value;
		} else if (par->getType() == Decimal::TYPE) {
			ptr->value += par->as<Decimal>()->value;
		} else return Null;
		return self;
	});
	IntegerClass->addMethod("-=", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto ptr = self->as<Integer>();
		if (other.size() != 1)
			return Null;
		auto par = other[0];
		if (par->getType() == Integer::TYPE) {
			ptr->value -= par->as<Integer>()->value;
		} else if (par->getType() == Decimal::TYPE) {
			ptr->value -= par->as<Decimal>()->value;
		} else return Null;
		return self;
	});
	IntegerClass->addMethod("toString", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return String::create(to_string(self->as<Integer>()->value), self->context);
	});
	DecimalClass->addMethod("+", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto value = self->as<Decimal>()->value;
		if (other.size() != 1)
			return Null;
		auto par = other[0];
		if (par->getType() == Integer::TYPE) {
			return Decimal::create(value + par->as<Integer>()->value, self->context);
		} else if (par->getType() == Decimal::TYPE) {
			return Decimal::create(value + par->as<Decimal>()->value, self->context);
		} else if (par->getType() == String::TYPE) {
			return String::create(to_string(value) + par->as<String>()->value, self->context);
		} else return Null;
	});
	DecimalClass->addMethod("-", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto value = self->as<Decimal>()->value;
		if (other.size() != 1)
			return Null;
		auto par = other[0];
		if (par->getType() == Integer::TYPE) {
			return Decimal::create(value - par->as<Integer>()->value, self->context);
		} else if (par->getType() == Decimal::TYPE) {
			return Decimal::create(value - par->as<Decimal>()->value, self->context);
		} else return Null;
	});
	DecimalClass->addMethod("+=", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto ptr = self->as<Decimal>();
		if (other.size() != 1)
			return Null;
		auto par = other[0];
		if (par->getType() == Integer::TYPE) {
			ptr->value += par->as<Integer>()->value;
		} else if (par->getType() == Decimal::TYPE) {
			ptr->value += par->as<Decimal>()->value;
		} else return Null;
		return self;
	});
	DecimalClass->addMethod("-=", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto ptr = self->as<Decimal>();
		if (other.size() != 1)
			return Null;
		auto par = other[0];
		if (par->getType() == Integer::TYPE) {
			ptr->value -= par->as<Integer>()->value;
		} else if (par->getType() == Decimal::TYPE) {
			ptr->value -= par->as<Decimal>()->value;
		} else return Null;
		return self;
	});
	DecimalClass->addMethod("toString", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return String::create(to_string(self->as<Decimal>()->value), self->context);
	});
	StringClass->addMethod("+", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		if (other.size() == 1) {
			shared_ptr<Object> var = other[0]->as<Object>();
			if (auto method = var->findMethod("toString")) {
				auto result = method->execute(var, {});
				if (result->getType() == String::TYPE) {
					return String::create(self->as<String>()->value + result->as<String>()->value, self->context);
				}
			}
		}
		return Null;
	});
	StringClass->addMethod("toString", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return self;
	});
	StringClass->addMethod("import", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto moduleName = self->as<String>()->value;
		try {
			Pool::execute(moduleName);
		} catch (const compile_error &e) {
			throw Pool::compile_fatal("Cannot import module \"" + moduleName + "\": " + string(e));
		}
		return Void;
	});
	VariableClass->addMethod("=", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		const shared_ptr<Object> value = other.size() == 1 ? other[0] : Void;
		self->as<Variable>()->setValue(value->as<Object>());
		return self;
	});
	VariableClass->addMethod("=>", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		const shared_ptr<Object> value = other.size() == 1 ? other[0] : Void;
		self->as<Variable>()->setValue(value->as<Object>());
		self->as<Variable>()->setImmutable(true);
		return self;
	});
	BlockClass->addMethod("toString", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		stringstream ss;
		auto ctx = self->context;
		ss << "{";
		if (!ctx->empty()) {
			for (auto &[name, value] : *ctx) {
				shared_ptr<Object> var = value->as<Object>();
				if (auto method = var->findMethod("toString")) {
					auto result = method->execute(var, {});
					ss << name << ":";
					if (result->getType() == String::TYPE) {
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
	FunClass->addMethod("classmethod", [](const shared_ptr<Object> &fself, const vector<shared_ptr<Object>> &fother) -> shared_ptr<Object> {
		return NativeFun::create([fself](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
			if (self->getType() == Class::TYPE) {
				return fself->as<Executable>()->execute(self->as<Object>(), other);
			} else {
				return fself->as<Executable>()->execute(self->as<Object>()->context->find("class"), other);
			}
		});
	});
	FunClass->addMethod("staticmethod", [](const shared_ptr<Object> &fself, const vector<shared_ptr<Object>> &fother) -> shared_ptr<Object> {
		return NativeFun::create([fself](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
			if (self->getType() == Class::TYPE) {
				return fself->as<Executable>()->execute(nullptr, other);
			} else {
				return fself->as<Executable>()->execute(nullptr, other);
			}
		});
	});
	FunClass->addMethod("toString", [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		stringstream ss;
		auto fun = self->as<Executable>();
		ss << "Function@" << fun->id << "(";
		if (!fun->params.empty()) {
			for (auto &param : fun->params) {
				ss << param << ",";
			}
			ss.seekp(-1, stringstream::cur); //remove last comma
		}
		ss << ")";
		return String::create(ss.str(), fun->context);
	});
	return true;
} catch (...) {
	cerr << "Initialization error" << endl;
	return false;
}

[[maybe_unused]]
const bool pool::initialized = initialize();
