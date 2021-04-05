#include "natives.hpp"
#include "pool.hpp"
#include "util/errors.hpp"

using namespace pool;

struct NativesImpl : public Natives {
	unordered_map<string, shared_ptr<Object>> natives;

	NativesImpl() {
		initialize();
	}

	void initialize() {
		initializeUtilities();
		initializeClass();
		initializeObject();
		initializeBool();
		initializeInteger();
		initializeDecimal();
		initializeString();
		initializeFun();
		initializeArray();
		initializeBlock();
	}

	void initializeUtilities() {
		add("Void", VoidClass);
		add("Nothing", NothingClass);
		add("void", Void);
		add("null", Null);
		addFun("throw", {{"message"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) -> shared_ptr<Object> {
			if (!(other[0]->getType() == String::TYPE))
				throw compile_error("throw argument must be a String", location);
			throw compile_error(other[0]->as<String>()->value, location);
		});
		addFun("tryCatch", {{"try"}, {"catch"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (!(other[0]->getType() == Block::TYPE))
				throw compile_error("tryCatch first argument must be a Block", location);
			if (!(other[1]->getType() == Fun::TYPE))
				throw compile_error("tryCatch second argument must be a Fun", location);
			try {
				return other[0]->as<Block>()->execute(location);
			} catch (const compile_error &error) {
				const shared_ptr<Fun> &ptr = other[1]->as<Fun>();
				return ptr->execute(ptr, {String::newInstance(ptr->context, location, error.what())}, location);
			}
		});
		addFun("input", {{"prompt"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) -> shared_ptr<Object> {
			if (!(other[0]->getType() == String::TYPE))
				throw compile_error("input argument must be a String", location);
			cout << other[0]->as<String>()->value;
			string in;
			cin >> in;
			return String::newInstance(self->context, location, in);
		});
	}

	void initializeClass() {
		add("Class", ClassClass);
		addFun("Class.extend", {{"this"}, {"name"}, {"body"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (!(other[0]->getType() == String::TYPE))
				throw compile_error("Class.extend: argument 'name' must be a String", location);
			if (!(other[1]->getType() == Block::TYPE))
				throw compile_error("Class.extend: argument 'body' must be a Block", location);
			auto name = other[0]->as<String>()->value;
			return self->as<Class>()->extend(nullptr, name, other[1]->as<Block>(), location);
		});
		addFun("Class.new", {{"this"}, {"arguments", true}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			auto cls = self->as<Class>();
			return cls->newInstance(self->context, location, other, cls);
		});
		addFun("Class.toString", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return String::newInstance(self->context, location, "Class:" + self->as<Class>()->name);
		});
		addFun("Class.subclassOf", {{"this"}, {"class"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (!(other[0]->getType() == Class::TYPE))
				throw compile_error("Class.subclassOf argument must be a Class", location);
			return self->as<Class>()->subclassOf(other[0]->as<Class>()) ? True : False;
		});
		addFun("Class.superclassOf", {{"this"}, {"class"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (!(other[0]->getType() == Class::TYPE))
				throw compile_error("Class.superclassOf argument must be a Class", location);
			return self->as<Class>()->superclassOf(other[0]->as<Class>()) ? True : False;
		});
	}

	void initializeObject() {
		add("Object", ObjectClass);
		addFun("Object.toString", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return String::newInstance(self->context, location, self->toString());
		});
		addFun("Object.type", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return String::newInstance(self->context, location, self->as<Object>()->context->toString());
		});
		addFun("Object.print", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			shared_ptr<Object> ptr = self->as<Object>();
			if (auto method = ptr->findMethod("toString")) {
				auto result = method->execute(ptr, {}, location);
				if (result->getType() == String::TYPE) {
					cout << result->as<String>()->value;
				}
			}
			return Void;
		});
		addFun("Object.println", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			shared_ptr<Object> ptr = self->as<Object>();
			if (auto method = ptr->findMethod("toString")) {
				auto result = method->execute(ptr, {}, location);
				if (result->getType() == String::TYPE) {
					cout << result->as<String>()->value << endl;
				}
			}
			return Void;
		});
		addFun("Object.instanceOf", {{"this"}, {"class"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (!(other[0]->getType() == Class::TYPE))
				throw compile_error("Object.instanceOf argument must be a Class", location);
			return self->as<Object>()->instanceOf(other[0]->as<Class>()) ? True : False;
		});
		addFun("Object.get", {{"this"}, {"key"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (!(other[0]->getType() == String::TYPE))
				throw compile_error("Object.get argument must be a String", location);
			shared_ptr<Object> ptr = self->as<Object>();
			const auto &key = other[0]->as<String>()->value;
			if (const auto &var = ptr->findLocal(key)) {
				return var->getValue();
			}
			return Void;
		});
		addFun("Object.set", {{"this"}, {"key"}, {"value"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) -> shared_ptr<Object> {
			if (!(other[0]->getType() == String::TYPE))
				throw compile_error("Object.set 'key' argument must be a String", location);
			shared_ptr<Object> ptr = self->as<Object>();
			const auto &key = other[0]->as<String>()->value;
			if (const auto &var = ptr->context->findLocal(key)) {
				if (!var->isImmutable()) {
					var->setValue(other[1]);
					return var;
				} else throw compile_error("Cannot set immutable variable '" + var->name + "'", location);
			} else return ptr->context->set(key, other[1]);
		});
		addFun("Object.delete", {{"this"}, {"key"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (!(other[0]->getType() == String::TYPE))
				throw compile_error("Object.delete argument must be a String", location);
			shared_ptr<Object> ptr = self->as<Object>();
			const auto &key = other[0]->as<String>()->value;
			if (const auto &var = ptr->findLocal(key)) {
				if (!var->isImmutable()) {
					ptr->remove(key);
				} else throw compile_error("Cannot remove immutable variable '" + var->name + "'", location);
			}
			return Void;
		});
	}

	void initializeBool() {
		add("true", True);
		add("false", False);
		add("Bool", BoolClass);
		addFun("Bool.toString", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return String::newInstance(self->context, location, string(self->as<Bool>()->value ? "true"
																							   : "false"));
		});
		addFun("Bool.!", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return !(self->as<Bool>()->value) ? True : False;
		});
		addFun("Bool.||", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			auto value = self->as<Bool>()->value;
			auto ptr = other[0];
			if (ptr->getType() == Bool::TYPE) {
				return value || ptr->as<Bool>()->value ? True : False;
			} else throw compile_error("Bool.|| argument must be a Number", location);
		});
		addFun("Bool.&&", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			auto value = self->as<Bool>()->value;
			auto ptr = other[0];
			if (ptr->getType() == Bool::TYPE) {
				return value && ptr->as<Bool>()->value ? True : False;
			} else throw compile_error("Bool.&& argument must be a Number", location);
		});
		addFun("Bool.==", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (other[0]->getType() == Bool::TYPE) {
				return (self->as<Bool>()->value == other[0]->as<Bool>()->value) ? True : False;
			} else return False;
		});
		addFun("Bool.then", {{"this"}, {"action"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (!(other[0]->getType() == Block::TYPE))
				throw compile_error("Bool.then argument must be a Block", location);
			auto block = other[0]->as<Block>();
			if (self->as<Bool>()->value) {
				return block->execute(location);
			}
			return Void;
		});
		addFun("Bool.thenElse", {{"this"}, {"trueAction"}, {"falseAction"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (!(other[0]->getType() == Block::TYPE))
				throw compile_error("Bool.then first argument must be a Block", location);
			if (!(other[1]->getType() == Block::TYPE))
				throw compile_error("Bool.then second argument must be a Block", location);
			if (self->as<Bool>()->value) {
				return other[0]->as<Block>()->execute(location);
			} else {
				return other[1]->as<Block>()->execute(location);
			}
		});
	}

	void initializeInteger() {
		this->add("Integer", IntegerClass);
		this->addFun("Integer.<", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			auto value = self->as<Integer>()->value;
			auto par = other[0];
			if (par->getType() == Integer::TYPE) {
				return (value < par->as<Integer>()->value) ? True : False;
			} else if (par->getType() == Decimal::TYPE) {
				return (value < par->as<Decimal>()->value) ? True : False;
			} else throw compile_error("Integer.< argument must be a Number", location);
		});
		this->addFun("Integer.==", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &oth = other[0];
			if (oth->getType() == Integer::TYPE) {
				return (self->as<Integer>()->value == oth->as<Integer>()->value) ? True : False;
			} else if (oth->getType() == Decimal::TYPE) {
				return (self->as<Integer>()->value == oth->as<Decimal>()->value) ? True : False;
			} else return False;
		});
		this->addFun("Integer.+", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			auto value = self->as<Integer>()->value;
			auto par = other[0];
			if (par->getType() == Integer::TYPE) {
				return Integer::newInstance(self->context, location, value + par->as<Integer>()->value);
			} else if (par->getType() == Decimal::TYPE) {
				return Decimal::newInstance(self->context, location, value + par->as<Decimal>()->value);
			} else if (par->getType() == String::TYPE) {
				return String::newInstance(self->context, location, to_string(value) + par->as<String>()->value);
			} else return Null;
		});
		this->addFun("Integer.-", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			auto value = self->as<Integer>()->value;
			auto par = other[0];
			if (par->getType() == Integer::TYPE) {
				return Integer::newInstance(self->context, location, value - par->as<Integer>()->value);
			} else if (par->getType() == Decimal::TYPE) {
				return Decimal::newInstance(self->context, location, value - par->as<Decimal>()->value);
			} else return Null;
		});
		this->addFun("Integer.toString", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return String::newInstance(self->context, location, to_string(self->as<Integer>()->value));
		});
	}

	void initializeDecimal() {
		this->add("Decimal", DecimalClass);
		this->addFun("Decimal.<", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			auto value = self->as<Decimal>()->value;
			auto par = other[0];
			if (par->getType() == Integer::TYPE) {
				return (value < par->as<Integer>()->value) ? True : False;
			} else if (par->getType() == Decimal::TYPE) {
				return (value < par->as<Decimal>()->value) ? True : False;
			} else throw compile_error("Decimal.< argument must be a Number", location);
		});
		this->addFun("Decimal.==", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &oth = other[0];
			if (oth->getType() == Integer::TYPE) {
				return (self->as<Decimal>()->value == oth->as<Integer>()->value) ? True : False;
			} else if (oth->getType() == Decimal::TYPE) {
				return (self->as<Decimal>()->value == oth->as<Decimal>()->value) ? True : False;
			} else return False;
		});
		this->addFun("Decimal.+", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			auto value = self->as<Decimal>()->value;
			auto par = other[0];
			if (par->getType() == Integer::TYPE) {
				return Decimal::newInstance(self->context, location, value + par->as<Integer>()->value);
			} else if (par->getType() == Decimal::TYPE) {
				return Decimal::newInstance(self->context, location, value + par->as<Decimal>()->value);
			} else if (par->getType() == String::TYPE) {
				return String::newInstance(self->context, location, to_string(value) + par->as<String>()->value);
			} else return Null;
		});
		this->addFun("Decimal.-", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			auto value = self->as<Decimal>()->value;
			auto par = other[0];
			if (par->getType() == Integer::TYPE) {
				return Decimal::newInstance(self->context, location, value - par->as<Integer>()->value);
			} else if (par->getType() == Decimal::TYPE) {
				return Decimal::newInstance(self->context, location, value - par->as<Decimal>()->value);
			} else return Null;
		});
		this->addFun("Decimal.toString", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return String::newInstance(self->context, location, to_string(self->as<Decimal>()->value));
		});
	}

	void initializeString() {
		this->add("String", StringClass);
		this->addFun("String.length", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return Integer::newInstance(self->context, location, self->as<String>()->value.length());
		});
		this->addFun("String.+", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			shared_ptr<Object> var = other[0]->as<Object>();
			if (auto method = var->findMethod("toString")) {
				auto result = method->execute(var, {}, location);
				if (result->getType() == String::TYPE) {
					const auto &value = self->as<String>()->value + result->as<String>()->value;
					return String::newInstance(self->context, location, value);
				}
			}
			return Null;
		});
		this->addFun("String.==", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (other[0]->getType() == String::TYPE) {
				return (self->as<String>()->value == other[0]->as<String>()->value) ? True : False;
			} else return False;
		});
		this->addFun("String.toString", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return String::newInstance(self->context, location, self->as<String>()->value);
		});
	}

	void initializeFun() {
		this->add("Fun", FunClass);
		this->addFun("Fun.classmethod", {{"this"}}, [](const shared_ptr<Object> &fself, const vector<shared_ptr<Object>> &fother, const Location &flocation) {
			return NativeFun::newInstance({{"this"}, {"args", true}}, [fself](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
				if (self->getType() == Class::TYPE) {
					return fself->as<Executable>()->execute(self->as<Object>(), other, location);
				} else {
					return fself->as<Executable>()->execute(self->as<Object>()->context->find("class"), other, location);
				}
			});
		});
		this->addFun("Fun.staticmethod", {{"this"}}, [](const shared_ptr<Object> &fself, const vector<shared_ptr<Object>> &fother, const Location &flocation) {
			return NativeFun::newInstance({{"this"}, {"args", true}}, [fself](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
				if (self->getType() == Class::TYPE) {
					return fself->as<Executable>()->execute(fself, other, location);
				} else {
					return fself->as<Executable>()->execute(fself, other, location);
				}
			});
		});
		this->addFun("Fun.toString", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			stringstream ss;
			auto fun = self->as<Fun>();
			ss << "Function@" << fun->id << "(";
			if (!fun->params.empty()) {
				for (auto &param : fun->params) {
					if (param.rest) ss << "...";
					ss << param.id << ",";
				}
				ss.seekp(-1, stringstream::cur); //remove last comma
			}
			ss << ")";
			return String::newInstance(self->context, location, ss.str());
		});
	}

	void initializeArray() {
		this->add("Array", ArrayClass);
		this->addFun("Array.init", {{"this"}, {"args", true}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			auto array = self->as<Array>();
			for (auto &value : other) {
				array->values.push_back(value->as<Object>());
			}
			return Void;
		});
		this->addFun("Array.at", {{"this"}, {"index"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			auto arg = other[0];
			if (arg->getType() == Integer::TYPE) {
				auto index = arg->as<Integer>()->value;
				auto array = self->as<Array>();
				if (index >= 0 && index < array->values.size()) {
					return array->values[index];
				} else throw compile_error("Array.at index out of range", location);
			} else throw compile_error("Array.at argument must be an integer", location);
		});
		this->addFun("Array.push", {{"this"}, {"args", true}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			auto array = self->as<Array>();
			for (auto &value : other) {
				array->values.push_back(value->as<Object>());
			}
			return Void;
		});
		this->addFun("Array.forEach", {{"this"}, {"action"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (other[0]->getType() == Fun::TYPE) {
				auto fun = other[0]->as<Fun>();
				auto array = self->as<Array>();
				size_t i = 0;
				for (auto &value : array->values) {
					if (fun->params.size() == 1)
						fun->execute(fun, {value}, location);
					else if (fun->params.size() == 2)
						fun->execute(fun, {value, Integer::newInstance(self->context, location, i)}, location);
					else if (fun->params.size() == 3)
						fun->execute(fun, {value, Integer::newInstance(self->context, location, i),
										   array}, location);
					else throw compile_error("Array.forEach argument Fun must have at most 3 paramenters", location);
					i++;
				}
				return self;
			} else throw compile_error("Array.forEach argument must be a Fun", location);
		});
		this->addFun("Array.toString", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			stringstream ss;
			auto ptr = self->as<Array>();
			ss << "[";
			if (!ptr->values.empty()) {
				for (auto &value : ptr->values) {
					if (auto method = value->findMethod("toString")) {
						auto str = method->execute(value, {}, location);
						if (str->getType() == String::TYPE)
							ss << str->as<String>()->value << ",";
						else
							ss << "?,";
					} else
						ss << "?,";
				}
				ss.seekp(-1, stringstream::cur); //remove last comma
			}
			ss << "]";
			return String::newInstance(self->context, location, ss.str());
		});
		this->addFun("Array.length", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return Integer::newInstance(self->context, location, self->as<Array>()->values.size());
		});
	}

	void initializeBlock() {
		this->add("Block", BlockClass);
		this->addFun("Block.whileDo", {{"this"}, {"action"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (other[0]->getType() == Block::TYPE) {
				auto block = other[0]->as<Block>();
				auto test = self->as<Block>();
				auto testResult = test->execute(location);
				if (!(testResult->getType() == Bool::TYPE))
					throw compile_error("this must return a Bool", location);
				while (testResult->as<Bool>()->value) {
					block->execute(location);
					testResult = test->execute(location);
					if (!(testResult->getType() == Bool::TYPE))
						throw compile_error("this must return a Bool", location);
				}
				return Void;
			} else throw compile_error("Block.whileDo argument must be a Block", location);
		});
		this->addFun("Block.doWhile", {{"this"}, {"action"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (other[0]->getType() == Block::TYPE) {
				auto block = other[0]->as<Block>();
				auto test = self->as<Block>();
				shared_ptr<Object> testResult;
				do {
					block->execute(location);
					testResult = test->execute(location);
					if (!(testResult->getType() == Bool::TYPE))
						throw compile_error("this must return a Bool", location);
				} while (testResult->as<Bool>()->value);
				return Void;
			} else throw compile_error("Block.whileDo argument must be a Block", location);
		});
	}

	bool add(const string &name, const shared_ptr<Object> &value) override {
		return natives.try_emplace(name, value).second;
	}

	bool addFun(const string &name, const vector<Fun::Param> &parameters, const NativeFun::method_t &code) override {
		return add(name, NativeFun::newInstance(parameters, code));
	}

	shared_ptr<Object> find(const string &name) override {
		const auto &iterator = natives.find(name);
		if (iterator != natives.end()) {
			return iterator->second;
		} else return nullptr;
	}
};

Natives &Natives::get() {
	static NativesImpl instance;
	return instance;
}
