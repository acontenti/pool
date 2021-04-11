#include <natives.hpp>
#include <poolstd.hpp>
#include <util/errors.hpp>
#include "util/dylib.hpp"
#include <ctgmath>
#include <iostream>

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
		initializeNumber();
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
		addFun("throw", {{"message", StringClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) -> shared_ptr<Object> {
			throw compile_error(other[0]->as<String>()->value, location);
		});
		addFun("tryCatch", {{"try", BlockClass}, {"catch", FunctionClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			try {
				return other[0]->as<Block>()->execute(location);
			} catch (const compile_error &error) {
				const auto &ptr = other[1]->as<Function>();
				return ptr->execute(ptr, {String::newInstance(ptr->context, location, error.what())}, location);
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
			return Void;
		});
		addFun("loadLibrary", {{"file", StringClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) -> shared_ptr<Object> {
			try {
				DynamicLibrariesManager::load(other[0]->as<String>()->value);
			} catch (const runtime_error &error) {
				throw compile_error(error.what(), location);
			}
			return Void;
		});
	}

	void initializeClass() {
		add("Class", ClassClass);
		addFun("Class.extend", {{"this"}, {"name", StringClass}, {"body", BlockClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &name = other[0]->as<String>()->value;
			const auto &cls = self->as<Class>();
			return cls->extend(cls->creator, name, other[1]->as<Block>(), location);
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
		add("Object", ObjectClass);
		addFun("Object.toString", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return String::newInstance(self->context, location, self->getRepr(location));
		});
		addFun("Object.getContextInfo", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return String::newInstance(self->context, location, self->context->toString(location));
		});
		addFun("Object.getClass", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return self->getClass();
		});
		addFun("Object.getRepr", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return String::newInstance(self->context, location, self->getDefaultRepr());
		});
		addFun("Object.print", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			cout << self->toString(location);
			return Void;
		});
		addFun("Object.println", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			cout << self->toString(location) << endl;
			return Void;
		});
		addFun("Object.instanceOf", {{"this"}, {"class", ClassClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return self->instanceOf(other[0]->as<Class>()) ? True : False;
		});
		addFun("Object.get", {{"this"}, {"key", StringClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &key = other[0]->as<String>()->value;
			if (const auto &var = self->findLocal(key)) {
				return var->getValue();
			}
			return Void;
		});
		addFun("Object.set", {{"this"}, {"key", StringClass}, {"value"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) -> shared_ptr<Object> {
			const auto &key = other[0]->as<String>()->value;
			if (const auto &var = self->context->findLocal(key)) {
				if (!var->isImmutable()) {
					var->setValue(other[1]);
				} else throw compile_error("Cannot set immutable variable '" + var->name + "'", location);
			} else self->context->set(key, other[1]);
			return other[1];
		});
		addFun("Object.delete", {{"this"}, {"key", StringClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &key = other[0]->as<String>()->value;
			if (const auto &var = self->findLocal(key)) {
				if (!var->isImmutable()) {
					self->remove(key);
				} else throw compile_error("Cannot remove immutable variable '" + var->name + "'", location);
			}
			return Void;
		});
		addFun("Object.==", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return self == other[0] ? True : False;
		});
	}

	void initializeBool() {
		add("true", True);
		add("false", False);
		add("Bool", BoolClass);
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
		addFun("Bool.then", {{"this"}, {"action", BlockClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return self->as<Bool>()->value ? other[0]->as<Block>()->execute(location) : Void;
		});
		addFun("Bool.thenElse", {{"this"}, {"trueAction", BlockClass}, {"falseAction", BlockClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return (self->as<Bool>()->value ? other[0]->as<Block>() : other[1]->as<Block>())->execute(location);
		});
	}

	void initializeNumber() {
		add("Number", NumberClass);
		addFun("Number.<", {{"this"}, {"other", NumberClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf(IntegerClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return (value < oth->as<Integer>()->value) ? True : False;
				} else if (oth->instanceOf(DecimalClass)) {
					return (value < oth->as<Decimal>()->value) ? True : False;
				}
			} else if (self->instanceOf(DecimalClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return (value < oth->as<Integer>()->value) ? True : False;
				} else if (oth->instanceOf(DecimalClass)) {
					return (value < oth->as<Decimal>()->value) ? True : False;
				}
			}
			throw compile_error("Number.< error: invalid subclass", location);
		});
		addFun("Number.==", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf(IntegerClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return (value == oth->as<Integer>()->value) ? True : False;
				} else if (oth->instanceOf(DecimalClass)) {
					return (value == oth->as<Decimal>()->value) ? True : False;
				} else return False;
			} else if (self->instanceOf(DecimalClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return (value == oth->as<Integer>()->value) ? True : False;
				} else if (oth->instanceOf(DecimalClass)) {
					return (value == oth->as<Decimal>()->value) ? True : False;
				} else return False;
			}
			throw compile_error("Number.== error: invalid subclass", location);
		});
		addFun("Number.+", {{"this"}, {"other"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf(IntegerClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return Integer::newInstance(self->context, location, value + oth->as<Integer>()->value);
				} else if (oth->instanceOf(DecimalClass)) {
					return Decimal::newInstance(self->context, location, value + oth->as<Decimal>()->value);
				} else if (oth->instanceOf(StringClass)) {
					return String::newInstance(self->context, location, to_string(value) + oth->as<String>()->value);
				} else throw compile_error("Number.+ argument must be a Number or a String", location);
			} else if (self->instanceOf(DecimalClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return Decimal::newInstance(self->context, location, value + oth->as<Integer>()->value);
				} else if (oth->instanceOf(DecimalClass)) {
					return Decimal::newInstance(self->context, location, value + oth->as<Decimal>()->value);
				} else if (oth->instanceOf(StringClass)) {
					return String::newInstance(self->context, location, to_string(value) + oth->as<String>()->value);
				} else throw compile_error("Number.+ argument must be a Number or a String", location);
			}
			throw compile_error("Number.+ error: invalid subclass", location);
		});
		addFun("Number.-", {{"this"}, {"other", NumberClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf(IntegerClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return Integer::newInstance(self->context, location, value - oth->as<Integer>()->value);
				} else if (oth->instanceOf(DecimalClass)) {
					return Decimal::newInstance(self->context, location, value - oth->as<Decimal>()->value);
				}
			} else if (self->instanceOf(DecimalClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return Decimal::newInstance(self->context, location, value - oth->as<Integer>()->value);
				} else if (oth->instanceOf(DecimalClass)) {
					return Decimal::newInstance(self->context, location, value - oth->as<Decimal>()->value);
				}
			}
			throw compile_error("Number.- error: invalid subclass", location);
		});
		addFun("Number.*", {{"this"}, {"other", NumberClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf(IntegerClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return Integer::newInstance(self->context, location, value * oth->as<Integer>()->value);
				} else if (oth->instanceOf(DecimalClass)) {
					return Decimal::newInstance(self->context, location, value * oth->as<Decimal>()->value);
				}
			} else if (self->instanceOf(DecimalClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return Decimal::newInstance(self->context, location, value * oth->as<Integer>()->value);
				} else if (oth->instanceOf(DecimalClass)) {
					return Decimal::newInstance(self->context, location, value * oth->as<Decimal>()->value);
				}
			}
			throw compile_error("Number.* error: invalid subclass", location);
		});
		addFun("Number./", {{"this"}, {"other", NumberClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf(IntegerClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return Decimal::newInstance(self->context, location,
												value / static_cast<long double>(oth->as<Integer>()->value));
				} else if (oth->instanceOf(DecimalClass)) {
					return Decimal::newInstance(self->context, location, value / oth->as<Decimal>()->value);
				}
			} else if (self->instanceOf(DecimalClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return Decimal::newInstance(self->context, location, value / oth->as<Integer>()->value);
				} else if (oth->instanceOf(DecimalClass)) {
					return Decimal::newInstance(self->context, location, value / oth->as<Decimal>()->value);
				}
			}
			throw compile_error("Number./ error: invalid subclass", location);
		});
		addFun("Number.%", {{"this"}, {"other", NumberClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf(IntegerClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return Integer::newInstance(self->context, location, value % oth->as<Integer>()->value);
				} else if (oth->instanceOf(DecimalClass)) {
					return Decimal::newInstance(self->context, location, fmod(value, oth->as<Decimal>()->value));
				}
			} else if (self->instanceOf(DecimalClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return Decimal::newInstance(self->context, location, fmod(value, oth->as<Integer>()->value));
				} else if (oth->instanceOf(DecimalClass)) {
					return Decimal::newInstance(self->context, location, fmod(value, oth->as<Decimal>()->value));
				}
			}
			throw compile_error("Number.% error: invalid subclass", location);
		});
		addFun("Number.**", {{"this"}, {"other", NumberClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf(IntegerClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Integer>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return Integer::newInstance(self->context, location, pow(value, oth->as<Integer>()->value));
				} else if (oth->instanceOf(DecimalClass)) {
					return Decimal::newInstance(self->context, location, pow(value, oth->as<Decimal>()->value));
				}
			} else if (self->instanceOf(DecimalClass)) {
				const auto &oth = other[0];
				const auto &value = self->as<Decimal>()->value;
				if (oth->instanceOf(IntegerClass)) {
					return Decimal::newInstance(self->context, location, pow(value, oth->as<Integer>()->value));
				} else if (oth->instanceOf(DecimalClass)) {
					return Decimal::newInstance(self->context, location, pow(value, oth->as<Decimal>()->value));
				}
			}
			throw compile_error("Number.** error: invalid subclass", location);
		});
		addFun("Number.abs", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf(IntegerClass)) {
				const auto &value = self->as<Integer>()->value;
				return Integer::newInstance(self->context, location, abs(value));
			} else if (self->instanceOf(DecimalClass)) {
				const auto &value = self->as<Decimal>()->value;
				return Decimal::newInstance(self->context, location, abs(value));
			}
			throw compile_error("Number.abs error: invalid subclass", location);
		});
		addFun("Number.neg", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf(IntegerClass)) {
				const auto &value = self->as<Integer>()->value;
				return Integer::newInstance(self->context, location, -value);
			} else if (self->instanceOf(DecimalClass)) {
				const auto &value = self->as<Decimal>()->value;
				return Decimal::newInstance(self->context, location, -value);
			}
			throw compile_error("Number.neg error: invalid subclass", location);
		});
		addFun("Number.getRepr", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			if (self->instanceOf(IntegerClass)) {
				return String::newInstance(self->context, location, to_string(self->as<Integer>()->value));
			} else if (self->instanceOf(DecimalClass)) {
				return String::newInstance(self->context, location, to_string(self->as<Decimal>()->value));
			}
			throw compile_error("Number.getDefaultRepr error: invalid subclass", location);
		});
	}

	void initializeInteger() {
		add("Integer", IntegerClass);
		addFun("Integer.|", {{"this"}, {"other", IntegerClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &value = self->as<Integer>()->value;
			return Integer::newInstance(self->context, location, value | other[0]->as<Integer>()->value);
		});
		addFun("Integer.&", {{"this"}, {"other", IntegerClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &value = self->as<Integer>()->value;
			return Integer::newInstance(self->context, location, value & other[0]->as<Integer>()->value);
		});
		addFun("Integer.^", {{"this"}, {"other", IntegerClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &value = self->as<Integer>()->value;
			return Integer::newInstance(self->context, location, value ^ other[0]->as<Integer>()->value);
		});
		addFun("Integer.~", {{"this"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			return Integer::newInstance(self->context, location, ~self->as<Integer>()->value);
		});
		addFun("Integer.<<", {{"this"}, {"other", IntegerClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &value = self->as<Integer>()->value;
			return Integer::newInstance(self->context, location, value << other[0]->as<Integer>()->value);
		});
		addFun("Integer.>>", {{"this"}, {"other", IntegerClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &value = self->as<Integer>()->value;
			return Integer::newInstance(self->context, location, value >> other[0]->as<Integer>()->value);
		});
	}

	void initializeDecimal() {
		add("Decimal", DecimalClass);
	}

	void initializeString() {
		add("String", StringClass);
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

	void initializeFun() {
		add("Function", FunctionClass);
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
				for (const auto &param : fun->params) {
					if (param.rest) ss << "...";
					ss << param.id;
					if (param.type) ss << ":" << param.type->name;
					ss << ",";
				}
				ss.seekp(-1, stringstream::cur); //remove last comma
			}
			ss << "]";
			return String::newInstance(self->context, location, ss.str());
		});
	}

	void initializeArray() {
		add("Array", ArrayClass);
		addFun("Array.at", {{"this"}, {"index", IntegerClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &index = other[0]->as<Integer>()->value;
			const auto &array = self->as<Array>();
			if (index >= 0 && index < array->values.size()) {
				return array->values[index];
			} else throw compile_error("Array.at: index out of range", location);

		});
		addFun("Array.push", {{"this"}, {"args", true}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &array = self->as<Array>();
			for (const auto &value : other) {
				array->values.push_back(value);
			}
			return Void;
		});
		addFun("Array.forEach", {{"this"}, {"action"}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &fun = other[0]->as<Function>();
			const auto &array = self->as<Array>();
			size_t i = 0;
			if (fun->params.empty()) {
				throw compile_error("Array.forEach: argument 'action' must have at least 1 paramenter", location);
			} else if (fun->params.size() == 1) {
				for (const auto &value : array->values) {
					fun->execute(fun, {value}, location);
				}
			} else if (fun->params.size() == 2) {
				for (const auto &value : array->values) {
					fun->execute(fun, {value, Integer::newInstance(self->context, location, i)}, location);
					i++;
				}
			} else if (fun->params.size() == 3) {
				for (const auto &value : array->values) {
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
				for (const auto &value : ptr->values) {
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

	void initializeBlock() {
		add("Block", BlockClass);
		addFun("Block.whileDo", {{"this"}, {"action", BlockClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &action = other[0]->as<Block>();
			const auto &test = self->as<Block>();
			auto testResult = test->execute(location);
			if (!(testResult->instanceOf(BoolClass)))
				throw compile_error("Block.whileDo: this must return a Bool", location);
			while (testResult->as<Bool>()->value) {
				action->execute(location);
				testResult = test->execute(location);
				if (!(testResult->instanceOf(BoolClass)))
					throw compile_error("Block.whileDo: this must return a Bool", location);
			}
			return Void;
		});
		addFun("Block.doWhile", {{"this"}, {"action", BlockClass}}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) {
			const auto &action = other[0]->as<Block>();
			const auto &test = self->as<Block>();
			shared_ptr<Object> testResult;
			do {
				action->execute(location);
				testResult = test->execute(location);
				if (!(testResult->instanceOf(BoolClass)))
					throw compile_error("Block.doWhile: this must return a Bool", location);
			} while (testResult->as<Bool>()->value);
			return Void;
		});
	}

	bool add(const string &name, const shared_ptr<Object> &value) override {
		return natives.try_emplace(name, value).second;
	}

	bool addFun(const string &name, const vector<Function::Param> &parameters, const NativeFunction::method_t &code) override {
		return add(name, NativeFunction::newInstance(parameters, code));
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
