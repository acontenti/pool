#include "pool.hpp"
#include "poolstd.hpp"

using namespace pool;

unordered_map<string, shared_ptr<Object>> pool::natives{};
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

void pool::initialize() {
	Context::global = Context::create(nullptr);
	Context::global->set("import", NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		if (other.size() == 1) {
			auto arg = other[0];
			if (arg->getType() == String::TYPE) {
				auto moduleName = other[0]->as<String>()->value;
				try {
					Pool::execute(moduleName);
				} catch (const compile_error &e) {
					throw execution_error("Cannot import module \"" + moduleName + "\": " + string(e));
				}
			} else throw execution_error("import argument must a String");
		} else throw execution_error("import needs one argument");
		return Void;
	}));
	natives["Class"] = ClassClass = make_shared<Class>(Context::create(Context::global), [](const shared_ptr<Context> &context, const any &creator, const any &name, const any &super) {
		return make_shared<Class>(context, creator, name, super.isNotNull() ? super.as<shared_ptr<Class>>() : nullptr);
	}, string(Class::TYPE), nullptr)->as<Class>(); // ObjectClass is not yet created, so we pass nullptr as super
	natives["Object"] = ObjectClass = ClassClass->newInstance(Context::global, {}, static_cast<Class::creator_t>([](const shared_ptr<Context> &context, const any &cls, const any &name, const any &super) -> shared_ptr<Object> {
		return make_shared<Object>(context, cls);
	}), string(Object::TYPE), nullptr)->as<Class>(); // ObjectClass is the base class, so it has no super
	ClassClass->super = ObjectClass; // Now ObjectClass exists, so we can assign it to ClassClass->super
	natives["Bool"] = BoolClass = ObjectClass->extend([](const shared_ptr<Context> &context, const any &value, const any &_a2 = nullptr, const any &_a3 = nullptr) {
		return make_shared<Bool>(context, value);
	}, string(Bool::TYPE));
	natives["Integer"] = IntegerClass = ObjectClass->extend([](const shared_ptr<Context> &context, const any &value, const any &_a2 = nullptr, const any &_a3 = nullptr) {
		return make_shared<Integer>(value, context);
	}, string(Integer::TYPE));
	natives["Decimal"] = DecimalClass = ObjectClass->extend([](const shared_ptr<Context> &context, const any &value, const any &_a2 = nullptr, const any &_a3 = nullptr) {
		return make_shared<Decimal>(value, context);
	}, string(Decimal::TYPE));
	natives["String"] = StringClass = ObjectClass->extend([](const shared_ptr<Context> &context, const any &value, const any &_a2 = nullptr, const any &_a3 = nullptr) {
		return make_shared<String>(context, value);
	}, string(String::TYPE));
	natives["Array"] = ArrayClass = ObjectClass->extend([](const shared_ptr<Context> &context, const any &_a1 = nullptr, const any &_a2 = nullptr, const any &_a3 = nullptr) {
		return make_shared<Array>(context);
	}, string(Array::TYPE));
	natives["Variable"] = VariableClass = ObjectClass->extend([](const shared_ptr<Context> &context, const any &name, const any &_a2 = nullptr, const any &_a3 = nullptr) {
		return make_shared<Variable>(name, Null, context, false);
	}, string(Variable::TYPE));
	natives["Block"] = BlockClass = ObjectClass->extend([](const shared_ptr<Context> &context, const any &calls, const any &_a2 = nullptr, const any &_a3 = nullptr) {
		return make_shared<Block>(calls, context);
	}, string(Block::TYPE));
	natives["Fun"] = FunClass = ObjectClass->extend([](const shared_ptr<Context> &context, const any &params, const any &calls, const any &_a3 = nullptr) {
		return make_shared<Fun>(params, calls, context);
	}, string(Fun::TYPE));
	natives["Void"] = VoidClass = ObjectClass->extend(nullptr, "Void");
	natives["Nothing"] = NothingClass = ObjectClass->extend(nullptr, "Nothing");
	natives["void"] = Void = VoidClass->newInstance(Context::global, {}, VoidClass);
	natives["null"] = Null = NothingClass->newInstance(Context::global, {}, NothingClass);
	natives["true"] = True = BoolClass->newInstance(Context::global, {}, true)->as<Bool>();
	natives["false"] = False = BoolClass->newInstance(Context::global, {}, false)->as<Bool>();
	natives["Class.extend"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		if (other.size() == 1) {
			if (other[0]->getType() == Block::TYPE) {
				auto name = string(Class::TYPE) + to_string(Class::COUNTER++);
				return self->as<Class>()->extend(nullptr, name, other[0]->as<Block>());
			} else throw execution_error("Class.extend: first argument must be a block");
		} else if (other.size() == 2) {
			if (other[0]->getType() == String::TYPE) {
				if (other[1]->getType() == Block::TYPE) {
					auto name = other[0]->as<String>()->value;
					return self->as<Class>()->extend(nullptr, name, other[1]->as<Block>());
				} else throw execution_error("Class.extend: second argument must be a block");
			} else throw execution_error("Class.extend: first argument must be a string");
		} else throw execution_error("Class.extend: invalid number of arguments");
	});
	natives["Class.new"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto cls = self->as<Class>();
		return cls->newInstance(self->context, other, cls);
	});
	natives["Class.toString"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) {
		return StringClass->newInstance(self->context, {}, "Class:" + self->as<Class>()->name);
	});
	natives["Object.toString"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) {
		return StringClass->newInstance(self->context, {}, self->toString());
	});
	natives["Object.type"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) {
		return StringClass->newInstance(self->context, {}, self->as<Object>()->context->toString());
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
	natives["Bool.toString"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return StringClass->newInstance(self->context, {}, string(self->as<Bool>()->value ? "true" : "false"));
	});
	natives["Bool.!"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return !(self->as<Bool>()->value) ? True : False;
	});
	natives["Integer.<"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto value = self->as<Integer>()->value;
		if (other.size() != 1)
			throw execution_error("Integer.< needs 1 argument");
		auto par = other[0];
		if (par->getType() == Integer::TYPE) {
			return (value < par->as<Integer>()->value) ? True : False;
		} else if (par->getType() == Decimal::TYPE) {
			return (value < par->as<Decimal>()->value) ? True : False;
		} else throw execution_error("Integer.< argument must be a Number");
	});
	natives["Integer.+"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto value = self->as<Integer>()->value;
		if (other.size() != 1)
			return Null;
		auto par = other[0];
		if (par->getType() == Integer::TYPE) {
			return IntegerClass->newInstance(self->context, {}, value + par->as<Integer>()->value);
		} else if (par->getType() == Decimal::TYPE) {
			return DecimalClass->newInstance(self->context, {}, value + par->as<Decimal>()->value);
		} else if (par->getType() == String::TYPE) {
			return StringClass->newInstance(self->context, {}, to_string(value) + par->as<String>()->value);
		} else return Null;
	});
	natives["Integer.-"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto value = self->as<Integer>()->value;
		if (other.size() != 1)
			return Null;
		auto par = other[0];
		if (par->getType() == Integer::TYPE) {
			return IntegerClass->newInstance(self->context, {}, value - par->as<Integer>()->value);
		} else if (par->getType() == Decimal::TYPE) {
			return DecimalClass->newInstance(self->context, {}, value - par->as<Decimal>()->value);
		} else return Null;
	});
	natives["Integer.toString"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return StringClass->newInstance(self->context, {}, to_string(self->as<Integer>()->value));
	});
	natives["Decimal.+"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto value = self->as<Decimal>()->value;
		if (other.size() != 1)
			return Null;
		auto par = other[0];
		if (par->getType() == Integer::TYPE) {
			return DecimalClass->newInstance(self->context, {}, value + par->as<Integer>()->value);
		} else if (par->getType() == Decimal::TYPE) {
			return DecimalClass->newInstance(self->context, {}, value + par->as<Decimal>()->value);
		} else if (par->getType() == String::TYPE) {
			return StringClass->newInstance(self->context, {}, to_string(value) + par->as<String>()->value);
		} else return Null;
	});
	natives["Decimal.-"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto value = self->as<Decimal>()->value;
		if (other.size() != 1)
			return Null;
		auto par = other[0];
		if (par->getType() == Integer::TYPE) {
			return DecimalClass->newInstance(self->context, {}, value - par->as<Integer>()->value);
		} else if (par->getType() == Decimal::TYPE) {
			return DecimalClass->newInstance(self->context, {}, value - par->as<Decimal>()->value);
		} else return Null;
	});
	natives["Decimal.toString"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		return StringClass->newInstance(self->context, {}, to_string(self->as<Decimal>()->value));
	});
	natives["String.+"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		if (other.size() == 1) {
			shared_ptr<Object> var = other[0]->as<Object>();
			if (auto method = var->findMethod("toString")) {
				auto result = method->execute(var, {});
				if (result->getType() == String::TYPE) {
					return StringClass->newInstance(self->context, {},
													self->as<String>()->value + result->as<String>()->value);
				}
			}
		}
		return Null;
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
		return StringClass->newInstance(self->context, {}, ss.str());
	});
	natives["Array.init"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto array = self->as<Array>();
		for (auto &value : other) {
			array->values.push_back(value->as<Object>());
		}
		return Void;
	});
	natives["Array.at"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		if (other.size() == 1) {
			auto arg = other[0];
			if (arg->getType() == Integer::TYPE) {
				auto index = arg->as<Integer>()->value;
				auto array = self->as<Array>();
				if (index >= 0 && index < array->values.size()) {
					return array->values[index];
				} else throw execution_error("Array.at index out of range");
			} else throw execution_error("Array.at argument must be an integer");
		} else throw execution_error("Array.at needs an argument");
	});
	natives["Array.push"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		auto array = self->as<Array>();
		for (auto &value : other) {
			array->values.push_back(value->as<Object>());
		}
		return Void;
	});
	natives["Array.forEach"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		if (other.size() == 1) {
			if (other[0]->getType() == Fun::TYPE) {
				auto fun = other[0]->as<Fun>();
				auto array = self->as<Array>();
				size_t i = 0;
				for (auto &value : array->values) {
					if (fun->params.size() == 1)
						fun->execute(fun, {value});
					else if (fun->params.size() == 2)
						fun->execute(fun, {value, IntegerClass->newInstance(self->context, {}, i)});
					else if (fun->params.size() == 3)
						fun->execute(fun, {value, IntegerClass->newInstance(self->context, {}, i), array});
					else throw execution_error("Array.forEach argument Fun must have at most 3 paramenters");
					i++;
				}
				return self;
			} else throw execution_error("Array.forEach argument must be a Fun");
		} else throw execution_error("Array.forEach needs 1 argument");
	});
	natives["Block.whileDo"] = NativeFun::create([](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) -> shared_ptr<Object> {
		if (other.size() == 1) {
			if (other[0]->getType() == Block::TYPE) {
				auto block = other[0]->as<Block>();
				auto test = self->as<Block>();
				while (test->execute(test, {})->as<Bool>()->value) {
					block->execute(block, {});
				}
				return Void;
			} else throw execution_error("Bool.whileDo argument must be a Block");
		} else throw execution_error("Bool.whileDo needs 1 argument");
	});
}