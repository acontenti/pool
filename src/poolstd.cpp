#include <iostream>
#include "poolstd.hpp"
#include "pool.hpp"

using namespace pool;

size_t Class::COUNTER = 0;
bool pool::debug = false;
vector<shared_ptr<Object>> pool::arguments{};
unordered_map<string, shared_ptr<Object>> pool::natives{};
const shared_ptr<Context> Context::global = Context::create(nullptr);
shared_ptr<Class> pool::ClassClass = nullptr;
shared_ptr<Class> pool::ObjectClass = nullptr;
shared_ptr<Class> pool::BoolClass = nullptr;
shared_ptr<Class> pool::IntegerClass = nullptr;
shared_ptr<Class> pool::DecimalClass = nullptr;
shared_ptr<Class> pool::StringClass = nullptr;
shared_ptr<Class> pool::ArrayClass = nullptr;
shared_ptr<Class> pool::VariableClass = nullptr;
shared_ptr<Class> pool::BlockClass = nullptr;
shared_ptr<Class> pool::FunClass = nullptr;
shared_ptr<Class> pool::VoidClass = nullptr;
shared_ptr<Class> pool::NothingClass = nullptr;
shared_ptr<Object> pool::Void = nullptr;
shared_ptr<Object> pool::Null = nullptr;
shared_ptr<Bool> pool::True = nullptr;
shared_ptr<Bool> pool::False = nullptr;

template<>
shared_ptr<Variable> Object::as() {
	return reinterpret_pointer_cast<Variable>(shared_from_this());
}

shared_ptr<Object> Context::find(const string &name) const {
	if (name == "=") return natives["Variable.="];
	if (name == "=>") return natives["Variable.=>"];
	auto iterator = heap.find(name);
	if (iterator != heap.end()) {
		return iterator->second;
	} else if (parent) {
		return parent->find(name);
	} else return nullptr;
}

shared_ptr<Object> Context::findLocal(const string &name) const {
	auto iterator = heap.find(name);
	if (iterator != heap.end()) {
		return iterator->second;
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

void pool::initialize() {
	natives["Variable.="] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		const shared_ptr<Object> value = other.size() == 1 ? other[0] : Void;
		self->as<Variable>()->setValue(value->as<Object>());
		return self;
	});
	natives["Variable.=>"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		const shared_ptr<Object> value = other.size() == 1 ? other[0] : Void;
		self->as<Variable>()->setValue(value->as<Object>());
		self->as<Variable>()->setImmutable(true);
		return self;
	});
	natives["Class"] = ClassClass = make_shared<Class>(Context::global, string(Class::TYPE), nullptr); //ObjectClass is not yet created
	natives["Object"] = ObjectClass = ClassClass->newInstance<Class>(Context::global, {}, string(Object::TYPE), nullptr);
	ClassClass->super = ObjectClass;
	natives["Bool"] = BoolClass = ObjectClass->extend(string(Bool::TYPE));
	natives["Integer"] = IntegerClass = ObjectClass->extend(string(Integer::TYPE));
	natives["Decimal"] = DecimalClass = ObjectClass->extend(string(Decimal::TYPE));
	natives["String"] = StringClass = ObjectClass->extend(string(String::TYPE));
	natives["Array"] = ArrayClass = ObjectClass->extend(string(Array::TYPE));
	natives["Variable"] = VariableClass = ObjectClass->extend(string(Variable::TYPE));
	natives["Block"] = BlockClass = ObjectClass->extend(string(Block::TYPE));
	natives["Fun"] = FunClass = ObjectClass->extend(string(Fun::TYPE));
	natives["Void"] = VoidClass = ObjectClass->extend("Void");
	natives["Nothing"] = NothingClass = ObjectClass->extend("Nothing");
	natives["void"] = Void = VoidClass->newInstance(Context::global);
	natives["null"] = Null = NothingClass->newInstance(Context::global);
	natives["true"] = True = BoolClass->newInstance<Bool>(Context::global, {}, true);
	natives["false"] = False = BoolClass->newInstance<Bool>(Context::global, {}, false);
	natives["Class.extend"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		if (other.size() == 1) {
			if (other[0]->getType() == Block::TYPE) {
				auto name = string(Class::TYPE) + to_string(Class::COUNTER++);
				return self->as<Class>()->extend(name, other[0]->as<Block>());
			} else throw execution_error("Class.extend: first argument must be a block");
		} else if (other.size() == 2) {
			if (other[0]->getType() == String::TYPE) {
				if (other[1]->getType() == Block::TYPE) {
					auto name = other[0]->as<String>()->value;
					return self->as<Class>()->extend(name, other[1]->as<Block>());
				} else throw execution_error("Class.extend: second argument must be a block");
			} else throw execution_error("Class.extend: first argument must be a string");
		} else throw execution_error("Class.extend: invalid number of arguments");
	});
	natives["Class.new"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto cls = self->as<Class>();
		return cls->newInstance(Context::create(self->context), other);
	});
	natives["Class.toString"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) {
		return String::create("Class:" + self->as<Class>()->name, self->context);
	});
	natives["Object.toString"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) {
		return String::create(self->toString(), self->context);
	});
	natives["Object.type"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) {
		return String::create(self->as<Object>()->context->toString(), self->context);
	});
	natives["Object.println"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		shared_ptr<Object> var = self->as<Object>();
		if (auto method = var->findMethod("toString")) {
			auto result = method->execute(var, {});
			if (result->getType() == String::TYPE) {
				cout << result->as<String>()->value << endl;
			}
		}
		return Void;
	});
	natives["Void.toString"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return String::create("void", self->context);
	});
	natives["Nothing.toString"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return String::create("null", self->context);
	});
	natives["Bool.toString"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return String::create(self->as<Bool>()->value ? "true" : "false", self->context);
	});
	natives["Bool.!"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return Bool::create(!self->as<Bool>()->value, self->context);
	});
	natives["Integer.+"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
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
	natives["Integer.-"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
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
	natives["Integer.++"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		self->as<Integer>()->value++;
		return self;
	});
	natives["Integer.--"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		self->as<Integer>()->value--;
		return self;
	});
	natives["Integer.+="] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
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
	natives["Integer.-="] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
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
	natives["Integer.toString"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return String::create(to_string(self->as<Integer>()->value), self->context);
	});
	natives["Decimal.+"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
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
	natives["Decimal.-"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
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
	natives["Decimal.+="] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
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
	natives["Decimal.-="] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
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
	natives["Decimal.toString"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return String::create(to_string(self->as<Decimal>()->value), self->context);
	});
	natives["String.+"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
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
	natives["String.toString"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return self;
	});
	natives["String.import"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto moduleName = self->as<String>()->value;
		try {
			Pool::execute(moduleName);
		} catch (const compile_error &e) {
			throw Pool::compile_fatal("Cannot import module \"" + moduleName + "\": " + string(e));
		}
		return Void;
	});
	natives["Fun.classmethod"] = NativeFun::create([](const shared_ptr<Object> &fself, const vector<shared_ptr<Object>> &fother) -> shared_ptr<Object> {
		return NativeFun::create([fself](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
			if (self->getType() == Class::TYPE) {
				return fself->as<Executable>()->execute(self->as<Object>(), other);
			} else {
				return fself->as<Executable>()->execute(self->as<Object>()->context->find("class"), other);
			}
		});
	});
	natives["Fun.staticmethod"] = NativeFun::create([](const shared_ptr<Object> &fself, const vector<shared_ptr<Object>> &fother) -> shared_ptr<Object> {
		return NativeFun::create([fself](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
			if (self->getType() == Class::TYPE) {
				return fself->as<Executable>()->execute(nullptr, other);
			} else {
				return fself->as<Executable>()->execute(nullptr, other);
			}
		});
	});
	natives["Fun.toString"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
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
}
