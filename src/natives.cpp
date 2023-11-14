/*
#include <util/errors.hpp>
#include "util/dylib.hpp"
#include <ctgmath>
#include <iostream>
#include <unordered_map>

using namespace pool;

struct NativesImpl {
	unordered_map<string, shared_ptr<any>> natives;

	NativesImpl() {
		initialize();
	}

	void initialize() {
		initializeSymbols();
		initializeUtilities();
		initializeClass();
		initializeObject();
		initializeModule();
		initializeBool();
		initializeNumber();
		initializeInteger();
		initializeString();
		initializeFunction();
		initializeArray();
	}

	void initializeSymbols() {
		add("Integer", Integer::CREATOR);
		add("Decimal", Decimal::CREATOR);
	}

	void initializeUtilities() {
		addFun("throw", {{"message", StringClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) -> shared_ptr<Object> {
			throw compile_error(other[0]->as<String>()->value, location);
		});
		addFun("tryCatch", {{"try", FunctionClass}, {"catch", FunctionClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			try {
				auto ptr = other[0]->as<Function>();
				return ptr->execute(ptr, {}, location);
			} catch (const compile_error &error) {
				const auto &ptr = other[1]->as<Function>();
				if (ptr->params.empty()) {
					return ptr->execute(ptr, {}, location);
				} else {
					return ptr->execute(ptr, {String::newInstance(ptr->context, location, error.what())}, location);
				}
			}
		});
		addFun("input", {{"prompt", StringClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) -> shared_ptr<Object> {
			cout << other[0]->as<String>()->value;
			string in;
			cin >> in;
			return String::newInstance(self->context, location, in);
		});
		addFun("loadLibraryFile", {{"file", StringClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) -> shared_ptr<Object> {
			try {
				DynamicLibrariesManager::loadPath(other[0]->as<String>()->value);
			} catch (const runtime_error &error) {
				throw compile_error(error.what(), location);
			}
			return Null;
		});
		addFun("loadLibrary", {{"file", StringClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) -> shared_ptr<Object> {
			try {
				DynamicLibrariesManager::load(other[0]->as<String>()->value);
			} catch (const runtime_error &error) {
				throw compile_error(error.what(), location);
			}
			return Null;
		});
	}

	void initializeClass() {
		addFun("Class.extend", {{"this"}, {"name", StringClass}, {"body", FunctionClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &name = other[0]->as<String>()->value;
			const auto &cls = self->as<Class>();
			return cls->extend(cls->creator, name, other[1]->as<Function>(), location);
		});
		addFun("Class.new", {{"this"}, {"arguments", true}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &cls = self->as<Class>();
			return cls->newInstance(self->context, location, other, cls);
		});
		addFun("Class.getSuper", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &cls = self->as<Class>();
			return cls->super ? cls->super : Null;
		});
		addFun("Class.getRepr", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return String::newInstance(self->context, location,
									   self->getDefaultRepr() + "[" + self->as<Class>()->name + "]");
		});
		addFun("Class.subclassOf", {{"this"}, {"class", ClassClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return self->as<Class>()->subclassOf(other[0]->as<Class>()) ? True : False;
		});
		addFun("Class.superclassOf", {{"this"}, {"class", ClassClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return self->as<Class>()->superclassOf(other[0]->as<Class>()) ? True : False;
		});
	}

	void initializeObject() {
		addFun("Object.getContextInfo", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &info = self->getContextInfo(location);
			return String::newInstance(self->context, location, info);
		});
		addFun("Object.getClass", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return self->getClass();
		});
		addFun("Object.getRepr", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return String::newInstance(self->context, location, self->getDefaultRepr());
		});
		addFun("Object.print", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			cout << self->toString(location);
			return Null;
		});
		addFun("Object.println", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			cout << self->toString(location) << endl;
			return Null;
		});
		addFun("Object.instanceOf", {{"this"}, {"class", ClassClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return self->instanceOf(other[0]->as<Class>()) ? True : False;
		});
		addFun("Object.get", {{"this"}, {"key", StringClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &key = other[0]->as<String>()->value;
			if (const auto &var = self->find(key)) {
				return var->getValue();
			}
			return Null;
		});
		addFun("Object.set", {{"this"}, {"key", StringClass}, {"value"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) -> shared_ptr<Object> {
			const auto &key = other[0]->as<String>()->value;
			self->context->set(key, other[1], false, location);
			return other[1];
		});
		addFun("Object.delete", {{"this"}, {"key", StringClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &key = other[0]->as<String>()->value;
			if (const auto &var = self->findLocal(key)) {
				if (!var->isImmutable()) {
					self->remove(key);
				} else throw compile_error("Cannot remove immutable variable '" + key + "'", location);
			}
			return Null;
		});
		addFun("Object.==", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return self == other[0] ? True : False;
		});
		addFun("Object.??", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return self != Null ? self : other[0];
		});
	}

	void initializeModule() {
		addFun("Module.inject", {{"this"}, {"moduleBlock", FunctionClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &imported = self->as<Module>();
			const auto &host = other[0];
			imported->inject(host->context, location);
			return Null;
		});
		addFun("Module.getId", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &module = self->as<Module>();
			return String::newInstance(self->context, location, module->id);
		});
	}

	void initializeBool() {
		addFun("Bool.new", {{"value", "Integer"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &value = other[0]->as<Integer>()->value;
			return value ? True : False;
		});
		addFun("Bool.getRepr", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return String::newInstance(self->context, location, self->as<Bool>()->value ? "true" : "false");
		});
		addFun("Bool.!", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return !(self->as<Bool>()->value) ? True : False;
		});
		addFun("Bool.||", {{"this"}, {"other", BoolClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return self->as<Bool>()->value || other[0]->as<Bool>()->value ? True : False;
		});
		addFun("Bool.&&", {{"this"}, {"other", BoolClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return self->as<Bool>()->value && other[0]->as<Bool>()->value ? True : False;
		});
		addFun("Bool.==", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (other[0]->instanceOf(BoolClass)) {
				return (self->as<Bool>()->value == other[0]->as<Bool>()->value) ? True : False;
			} else return False;
		});
		addFun("Bool.then", {{"this"}, {"action", FunctionClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			auto fun = other[0]->as<Function>();
			return self->as<Bool>()->value ? fun->execute(fun, {}, location) : Null;
		});
		addFun("Bool.thenElse", {{"this"}, {"trueAction", FunctionClass}, {"falseAction", FunctionClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			auto fun = (self->as<Bool>()->value ? other[0] : other[1])->as<Function>();
			return fun->execute(fun, {}, location);
		});
		addFun("Bool.?", {{"this"}, {"trueValue"}, {"falseValue"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return self->as<Bool>()->value ? other[0] : other[1];
		});
	}

	void initializeNumber() {
		addFun("Number.<", {{"this"}, {"other", "Number"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf("Integer")) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf("Integer")) {
					return (value < oth->as<Integer>()->value) ? True : False;
				} else if (oth->instanceOf("Decimal")) {
					return (value < oth->as<Decimal>()->value) ? True : False;
				}
			} else if (self->instanceOf("Decimal")) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf("Integer")) {
					return (value < oth->as<Integer>()->value) ? True : False;
				} else if (oth->instanceOf("Decimal")) {
					return (value < oth->as<Decimal>()->value) ? True : False;
				}
			}
			throw compile_error("Number.< error: invalid subclass", location);
		});
		addFun("Number.==", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf("Integer")) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf("Integer")) {
					return (value == oth->as<Integer>()->value) ? True : False;
				} else if (oth->instanceOf("Decimal")) {
					return (value == oth->as<Decimal>()->value) ? True : False;
				} else return False;
			} else if (self->instanceOf("Decimal")) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf("Integer")) {
					return (value == oth->as<Integer>()->value) ? True : False;
				} else if (oth->instanceOf("Decimal")) {
					return (value == oth->as<Decimal>()->value) ? True : False;
				} else return False;
			}
			throw compile_error("Number.== error: invalid subclass", location);
		});
		addFun("Number.+", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf("Integer")) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf("Integer")) {
					return Integer::newInstance(self->context, location, value + oth->as<Integer>()->value);
				} else if (oth->instanceOf("Decimal")) {
					return Decimal::newInstance(self->context, location, value + oth->as<Decimal>()->value);
				} else if (oth->instanceOf(StringClass)) {
					return String::newInstance(self->context, location, to_string(value) + oth->as<String>()->value);
				} else throw compile_error("Number.+ argument must be a Number or a String", location);
			} else if (self->instanceOf("Decimal")) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf("Integer")) {
					return Decimal::newInstance(self->context, location, value + oth->as<Integer>()->value);
				} else if (oth->instanceOf("Decimal")) {
					return Decimal::newInstance(self->context, location, value + oth->as<Decimal>()->value);
				} else if (oth->instanceOf(StringClass)) {
					return String::newInstance(self->context, location, to_string(value) + oth->as<String>()->value);
				} else throw compile_error("Number.+ argument must be a Number or a String", location);
			}
			throw compile_error("Number.+ error: invalid subclass", location);
		});
		addFun("Number.-", {{"this"}, {"other", "Number"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf("Integer")) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf("Integer")) {
					return Integer::newInstance(self->context, location, value - oth->as<Integer>()->value);
				} else if (oth->instanceOf("Decimal")) {
					return Decimal::newInstance(self->context, location, value - oth->as<Decimal>()->value);
				}
			} else if (self->instanceOf("Decimal")) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf("Integer")) {
					return Decimal::newInstance(self->context, location, value - oth->as<Integer>()->value);
				} else if (oth->instanceOf("Decimal")) {
					return Decimal::newInstance(self->context, location, value - oth->as<Decimal>()->value);
				}
			}
			throw compile_error("Number.- error: invalid subclass", location);
		});
		addFun("Number.*", {{"this"}, {"other", "Number"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf("Integer")) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf("Integer")) {
					return Integer::newInstance(self->context, location, value * oth->as<Integer>()->value);
				} else if (oth->instanceOf("Decimal")) {
					return Decimal::newInstance(self->context, location, value * oth->as<Decimal>()->value);
				}
			} else if (self->instanceOf("Decimal")) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf("Integer")) {
					return Decimal::newInstance(self->context, location, value * oth->as<Integer>()->value);
				} else if (oth->instanceOf("Decimal")) {
					return Decimal::newInstance(self->context, location, value * oth->as<Decimal>()->value);
				}
			}
			throw compile_error("Number.* error: invalid subclass", location);
		});
		addFun("Number./", {{"this"}, {"other", "Number"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf("Integer")) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf("Integer")) {
					return Decimal::newInstance(self->context, location,
												value / static_cast<long double>(oth->as<Integer>()->value));
				} else if (oth->instanceOf("Decimal")) {
					return Decimal::newInstance(self->context, location, value / oth->as<Decimal>()->value);
				}
			} else if (self->instanceOf("Decimal")) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf("Integer")) {
					return Decimal::newInstance(self->context, location, value / oth->as<Integer>()->value);
				} else if (oth->instanceOf("Decimal")) {
					return Decimal::newInstance(self->context, location, value / oth->as<Decimal>()->value);
				}
			}
			throw compile_error("Number./ error: invalid subclass", location);
		});
		addFun("Number.%", {{"this"}, {"other", "Number"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf("Integer")) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf("Integer")) {
					return Integer::newInstance(self->context, location, value % oth->as<Integer>()->value);
				} else if (oth->instanceOf("Decimal")) {
					return Decimal::newInstance(self->context, location, fmod(value, oth->as<Decimal>()->value));
				}
			} else if (self->instanceOf("Decimal")) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf("Integer")) {
					return Decimal::newInstance(self->context, location, fmod(value, oth->as<Integer>()->value));
				} else if (oth->instanceOf("Decimal")) {
					return Decimal::newInstance(self->context, location, fmod(value, oth->as<Decimal>()->value));
				}
			}
			throw compile_error("Number.% error: invalid subclass", location);
		});
		addFun("Number.**", {{"this"}, {"other", "Number"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf("Integer")) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf("Integer")) {
					return Integer::newInstance(self->context, location, pow(value, oth->as<Integer>()->value));
				} else if (oth->instanceOf("Decimal")) {
					return Decimal::newInstance(self->context, location, pow(value, oth->as<Decimal>()->value));
				}
			} else if (self->instanceOf("Decimal")) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf("Integer")) {
					return Decimal::newInstance(self->context, location, pow(value, oth->as<Integer>()->value));
				} else if (oth->instanceOf("Decimal")) {
					return Decimal::newInstance(self->context, location, pow(value, oth->as<Decimal>()->value));
				}
			}
			throw compile_error("Number.** error: invalid subclass", location);
		});
		addFun("Number.abs", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf("Integer")) {
				const auto &value = self->as<Integer>()->value;
				return Integer::newInstance(self->context, location, abs(value));
			} else if (self->instanceOf("Decimal")) {
				const auto &value = self->as<Decimal>()->value;
				return Decimal::newInstance(self->context, location, abs(value));
			}
			throw compile_error("Number.abs error: invalid subclass", location);
		});
		addFun("Number.neg", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf("Integer")) {
				const auto &value = self->as<Integer>()->value;
				return Integer::newInstance(self->context, location, -value);
			} else if (self->instanceOf("Decimal")) {
				const auto &value = self->as<Decimal>()->value;
				return Decimal::newInstance(self->context, location, -value);
			}
			throw compile_error("Number.neg error: invalid subclass", location);
		});
		addFun("Number.getRepr", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf("Integer")) {
				return String::newInstance(self->context, location, to_string(self->as<Integer>()->value));
			} else if (self->instanceOf("Decimal")) {
				return String::newInstance(self->context, location, to_string(self->as<Decimal>()->value));
			}
			throw compile_error("Number.getDefaultRepr error: invalid subclass", location);
		});
	}

	void initializeInteger() {
		addFun("Integer.|", {{"this"}, {"other", "Integer"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &value = self->as<Integer>()->value;
			return Integer::newInstance(self->context, location, value | other[0]->as<Integer>()->value);
		});
		addFun("Integer.&", {{"this"}, {"other", "Integer"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &value = self->as<Integer>()->value;
			return Integer::newInstance(self->context, location, value & other[0]->as<Integer>()->value);
		});
		addFun("Integer.^", {{"this"}, {"other", "Integer"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &value = self->as<Integer>()->value;
			return Integer::newInstance(self->context, location, value ^ other[0]->as<Integer>()->value);
		});
		addFun("Integer.~", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return Integer::newInstance(self->context, location, ~self->as<Integer>()->value);
		});
		addFun("Integer.<<", {{"this"}, {"other", "Integer"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &value = self->as<Integer>()->value;
			return Integer::newInstance(self->context, location, value << other[0]->as<Integer>()->value);
		});
		addFun("Integer.>>", {{"this"}, {"other", "Integer"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &value = self->as<Integer>()->value;
			return Integer::newInstance(self->context, location, value >> other[0]->as<Integer>()->value);
		});
	}

	void initializeString() {
		addFun("String.length", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return Integer::newInstance(self->context, location, self->as<String>()->value.length());
		});
		addFun("String.+", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &oth = other[0];
			const auto &value = self->as<String>()->value;
			if (oth->instanceOf(StringClass)) {
				return String::newInstance(self->context, location, value + oth->as<String>()->value);
			} else {
				return String::newInstance(self->context, location, value + oth->toString(location));
			}
		});
		addFun("String.==", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (other[0]->instanceOf(StringClass)) {
				return (self->as<String>()->value == other[0]->as<String>()->value) ? True : False;
			} else return False;
		});
		addFun("String.toString", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return self;
		});
		addFun("String.getRepr", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return String::newInstance(self->context, location, "\"" + self->as<String>()->value + "\"");
		});
	}

	void initializeFunction() {
		addFun("Function.classmethod", {{"this"}}, [](const shared_ptr<Object> &fself, const vector<shared_ptr<Object>> &fother, const Location &flocation) {
			const auto &ptr = fself->as<Function>();
			return NativeFunction::newInstance(ptr->params, [ptr](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
				return ptr->execute(self->instanceOf(ClassClass) ? self : self->getClass(), other, location);
			});
		});
		addFun("Function.staticmethod", {{"this"}}, [](const shared_ptr<Object> &fself, const vector<shared_ptr<Object>> &fother, const Location &flocation) {
			const auto &ptr = fself->as<Function>();
			vector<Function::Param> params = {{"ref"}};
			params.insert(params.end(), ptr->params.begin(), ptr->params.end());
			return NativeFunction::newInstance(params, [ptr](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
				return ptr->execute(ptr, other, location);
			});
		});
		addFun("Function.getRepr", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &fun = self->as<Function>();
			stringstream ss;
			ss << self->getDefaultRepr() << "[";
			if (!fun->params.empty()) {
				for (const auto &param: fun->params) {
					if (param.rest) ss << "...";
					ss << param.id;
					if (param.type) ss << ":" << param.type->name;
					else if (!param.typeName.empty()) ss << ":" << param.typeName;
					ss << ",";
				}
				ss.seekp(-1, stringstream::cur); //remove last comma
			}
			ss << "]";
			return String::newInstance(self->context, location, ss.str());
		});
		addFun("Function.whileDo", {{"this"}, {"action", FunctionClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &action = other[0]->as<Function>();
			const auto &test = self->as<Function>();
			auto testResult = test->execute(test, {}, location);
			if (!(testResult->instanceOf(BoolClass)))
				throw compile_error("Function.whileDo: this must return a Bool", location);
			while (testResult->as<Bool>()->value) {
				action->execute(action, {}, location);
				testResult = test->execute(test, {}, location);
				if (!(testResult->instanceOf(BoolClass)))
					throw compile_error("Function.whileDo: this must return a Bool", location);
			}
			return Null;
		});
		addFun("Function.doWhile", {{"this"}, {"action", FunctionClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &action = other[0]->as<Function>();
			const auto &test = self->as<Function>();
			shared_ptr<Object> testResult;
			do {
				action->execute(action, {}, location);
				testResult = test->execute(test, {}, location);
				if (!(testResult->instanceOf(BoolClass)))
					throw compile_error("Function.doWhile: this must return a Bool", location);
			} while (testResult->as<Bool>()->value);
			return Null;
		});
		addFun("Function.return", {{"this"}, {"value", true}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (const auto &fun = self->as<Function>()) {
				throw Function::return_exception(fun, other.empty() ? Null : other[0]);
			}
			return Null;
		});
	}

	void initializeArray() {
		addFun("Array.at", {{"this"}, {"index", "Integer"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &index = other[0]->as<Integer>()->value;
			const auto &array = self->as<Array>();
			if (index >= 0 && index < array->values.size()) {
				return array->values[index];
			} else throw compile_error("Array.at: index out of range", location);

		});
		addFun("Array.push", {{"this"}, {"args", true}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &array = self->as<Array>();
			for (const auto &value: other) {
				array->values.push_back(value);
			}
			return Null;
		});
		addFun("Array.forEach", {{"this"}, {"action"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &fun = other[0]->as<Function>();
			const auto &array = self->as<Array>();
			size_t i = 0;
			if (fun->params.empty()) {
				throw compile_error("Array.forEach: argument 'action' must have at least 1 paramenter", location);
			} else if (fun->params.size() == 1) {
				for (const auto &value: array->values) {
					fun->execute(fun, {value}, location);
				}
			} else if (fun->params.size() == 2) {
				for (const auto &value: array->values) {
					fun->execute(fun, {value, Integer::newInstance(self->context, location, i)}, location);
					i++;
				}
			} else if (fun->params.size() == 3) {
				for (const auto &value: array->values) {
					fun->execute(fun, {value, Integer::newInstance(self->context, location, i), array}, location);
					i++;
				}
			} else throw compile_error("Array.forEach: argument 'action' must have at most 3 paramenters", location);
			return self;
		});
		addFun("Array.getRepr", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &value = self->getDefaultRepr() + "[" + to_string(self->as<Array>()->values.size()) + "]";
			return String::newInstance(self->context, location, value);
		});
		addFun("Array.toString", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			stringstream ss;
			const auto &ptr = self->as<Array>();
			ss << "[";
			if (!ptr->values.empty()) {
				for (const auto &value: ptr->values) {
					ss << value->getRepr(location) << ",";
				}
				ss.seekp(-1, stringstream::cur); //remove last comma
			}
			ss << "]";
			return String::newInstance(self->context, location, ss.str());
		});
		addFun("Array.length", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return Integer::newInstance(self->context, location, self->as<Array>()->values.size());
		});
	}

	inline bool addValue(const string &name, const any &value) {
		return natives.try_emplace(name, make_shared<any>(value)).second;
	}

	bool add(const string &name, const shared_ptr<Object> &value) {
		return addValue(name, value);
	}

	bool add(const string &name, const Class::creator_t &value) {
		return addValue(name, value);
	}

	bool addFun(const string &name, const vector<Function::Param> &parameters, const NativeFunction::method_t &code) {
		return add(name, NativeFunction::newInstance(parameters, code));
	}

	any find(const string &name) {
		const auto &iterator = natives.find(name);
		if (iterator != natives.end()) {
			return *iterator->second;
		} else return any();
	}
};
*/
