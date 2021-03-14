#pragma once

#include <functional>
#include "util.hpp"
#include "callable.hpp"
#include "context.hpp"
#include "parser.hpp"

using namespace std;

namespace pool {
	class Class;

	class Variable;

	class Object;

	class Bool;

	class Block;

	class Executable;

	extern bool debug;
	extern vector<shared_ptr<Object>> arguments;
	extern unordered_map<string, shared_ptr<Object>> natives;
	extern shared_ptr<Class> ClassClass;
	extern shared_ptr<Class> ObjectClass;
	extern shared_ptr<Class> BoolClass;
	extern shared_ptr<Class> IntegerClass;
	extern shared_ptr<Class> DecimalClass;
	extern shared_ptr<Class> StringClass;
	extern shared_ptr<Class> ArrayClass;
	extern shared_ptr<Class> VariableClass;
	extern shared_ptr<Class> BlockClass;
	extern shared_ptr<Class> FunClass;
	extern shared_ptr<Class> VoidClass;
	extern shared_ptr<Class> NothingClass;
	extern shared_ptr<Object> Void;
	extern shared_ptr<Object> Null;
	extern shared_ptr<Bool> True;
	extern shared_ptr<Bool> False;

	class Object : public enable_shared_from_this<Object> {
		shared_ptr<Object> getVariableValue();

	public:
		constexpr static const string_view TYPE = "Object";
		const size_t id;
		const shared_ptr<Class> cls;
		const shared_ptr<Context> context;

		Object(shared_ptr<Class> cls, const shared_ptr<Context> &context);

		virtual string_view getType();

		inline string toString() {
			return string(getType()) + "@" + to_string(id);
		};

		template<class T>
		shared_ptr<T> as() {
			if (isVariable())
				return reinterpret_pointer_cast<T>(getVariableValue());
			return reinterpret_pointer_cast<T>(shared_from_this());
		}

		virtual shared_ptr<Executable> findMethod(const string &methodName) const;

		virtual shared_ptr<Object> find(const string &name) const;

		virtual shared_ptr<Object> findLocal(const string &id) const;

		virtual bool isVariable() const {
			return false;
		}
	};

	template<>
	shared_ptr<Variable> Object::as<Variable>();

	class Class : public Object {
	public:
		constexpr static const string_view TYPE = "Class";
		static std::size_t COUNTER;
		size_t instances = 0;
		string name;
		shared_ptr<Class> super;

		Class(const shared_ptr<Context> &context, string name, shared_ptr<Class> super);

		shared_ptr<Object> find(const string &id) const override {
			if (auto local = findInClass(id)) {
				return local;
			}
			if (cls) {
				return cls->find(id);
			}
			return nullptr;
		}

		shared_ptr<Object> findInClass(const string &id) const {
			if (auto local = context->find(id)) {
				return local;
			}
			if (super) {
				return super->findInClass(id);
			}
			return nullptr;
		};

		template<class T, typename std::enable_if<std::is_base_of<Object, T>::value>::type * = nullptr, typename... Args>
		shared_ptr<T> newInstance(const shared_ptr<Context> &context, const vector<shared_ptr<Object>> &other = {}, Args &&... args) {
			auto ptr = make_shared<T>(context, args...);
			callInit(ptr, other);
			return ptr;
		}

		shared_ptr<Class> extend(const string &className, const shared_ptr<Block> &other = nullptr);

		static void callInit(const shared_ptr<Object> &ptr, const vector<shared_ptr<Object>> &other);
	};

	template<>
	shared_ptr<Object> Class::newInstance<Object>(const shared_ptr<Context> &context, const vector<shared_ptr<Object>> &other);

	class Bool : public Object {
	public:
		bool value;
		constexpr static const string_view TYPE = "Bool";

		Bool(const shared_ptr<Context> &context, bool value) : Object(BoolClass, context), value(value) {}

		static shared_ptr<Bool> create(const bool &value, const shared_ptr<Context> &context) {
			return make_shared<Bool>(context, value);
		}
	};

	class Integer : public Object {
	public:
		int64_t value;
		constexpr static const string_view TYPE = "Integer";

		Integer(int64_t value, const shared_ptr<Context> &context) : Object(IntegerClass, context), value(value) {}

		static shared_ptr<Integer> create(const int64_t &value, const shared_ptr<Context> &context) {
			return make_shared<Integer>(value, context);
		}
	};

	class Decimal : public Object {
	public:
		double value;
		constexpr static const string_view TYPE = "Decimal";

		Decimal(double value, const shared_ptr<Context> &context) : Object(DecimalClass, context), value(value) {}

		static shared_ptr<Decimal> create(const double &value, const shared_ptr<Context> &context) {
			return make_shared<Decimal>(value, context);
		}
	};

	class String : public Object {
	public:
		constexpr static const string_view TYPE = "String";
		string value;

		String(const shared_ptr<Context> &context, string value) : Object(StringClass, context), value(move(value)) {}

		static shared_ptr<String> create(const string &value, const shared_ptr<Context> &context) {
			return StringClass->newInstance<String>(context, {}, value);
		}
	};


	class Variable : public Object {
		shared_ptr<Object> value;
		bool immutable = false;
	public:
		constexpr static const string_view TYPE = "Variable";
		const string name;

		void setImmutable(bool _immutable) {
			immutable = _immutable;
		}

		const shared_ptr<Object> &getValue() const {
			return value;
		}

		void setValue(const shared_ptr<Object> &val) {
			if (!immutable)
				value = val;
			else throw execution_error("Cannot assign immutable variable \"" + name + "\"");
		}

		shared_ptr<Executable> findMethod(const string &methodName) const override {
			return value->findMethod(methodName);
		}

		shared_ptr<Object> find(const string &id) const override {
			return value->find(id);
		}

		shared_ptr<Object> findLocal(const string &id) const override {
			return value->findLocal(id);
		}


		string_view getType() override {
			return value->getType();
		}

		bool isVariable() const override {
			return true;
		}

		Variable(string name, shared_ptr<Object> value, const shared_ptr<Context> &context, bool immutable)
				: Object(VariableClass, context), name(move(name)), value(move(value)), immutable(immutable) {
		}

		static shared_ptr<Variable> create(const string &name, const shared_ptr<Context> &context) {
			return make_shared<Variable>(name, Null, context, false);
		}
	};

	class Executable : public Object {
	public:
		vector<string> params;

		Executable(const vector<string> &params, const shared_ptr<Context> &context, const shared_ptr<Class> &cls)
				: Object(cls, context), params(params) {}

		virtual shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) = 0;
	};

	using method_t = function<shared_ptr<Object>(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other)>;

	class NativeFun : public Executable {
	public:
		method_t code;

		NativeFun(method_t code, const shared_ptr<Context> &context)
				: Executable({"this", "other"}, context, FunClass), code(move(code)) {}

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) override {
			auto returnValue = code(self, other);
			return returnValue;
		}

		static shared_ptr<NativeFun> create(const method_t &code) {
			return make_shared<NativeFun>(code, Context::global);
		}
	};

	class Fun : public Executable {
	public:
		constexpr static const string_view TYPE = "Fun";
		vector<shared_ptr<Callable>> calls;

		Fun(const vector<string> &params, vector<shared_ptr<Callable>> calls, const shared_ptr<Context> &context)
				: Executable(params, context, FunClass), calls(move(calls)) {
			for (auto &param : params) {
				context->add(param);
			}
		}

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) override {
			shared_ptr<Object> returnValue = Void;
			vector<shared_ptr<Object>> args;
			if (self) {
				auto ptr = self->as<Object>();
				if (ptr != shared_from_this()) {
					args.reserve(other.size() + 1);
					args.push_back(ptr);
				}
			} else {
				args.reserve(other.size());
			}
			args.insert(args.end(), other.begin(), other.end());
			if (params.size() != args.size())
				throw execution_error("Parameters number does not match arguments number");
			context->associate(params, args);
			for (auto &call: calls) {
				returnValue = call->invoke();
			}
			return returnValue;
		}

		static shared_ptr<Fun> create(const vector<string> &params, const vector<shared_ptr<Callable>> &calls, const shared_ptr<Context> &context) {
			return make_shared<Fun>(params, calls, context);
		}
	};

	class Block : public Executable, public Callable {
	public:
		constexpr static const string_view TYPE = "Block";
		vector<shared_ptr<Callable>> calls;

		Block(vector<shared_ptr<Callable>> calls, const shared_ptr<Context> &context)
				: Executable({}, context, BlockClass), calls(move(calls)) {}

		shared_ptr<Object> invoke() override {
			shared_ptr<Object> returnValue = Void;
			for (auto &call: calls) {
				call->invoke();
			}
			return returnValue;
		}

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) override {
			return invoke();
		}

		static shared_ptr<Block> create(const vector<shared_ptr<Callable>> &calls, const shared_ptr<Context> &context) {
			return make_shared<Block>(calls, context);
		}
	};

	class Array : public Object, public Callable {
	public:
		constexpr static const string_view TYPE = "Array";
		vector<shared_ptr<Callable>> calls;
		vector<shared_ptr<Object>> values;

		Array(vector<shared_ptr<Callable>> calls, const shared_ptr<Context> &context)
				: Object(ArrayClass, context), calls(move(calls)) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Array> create(const vector<shared_ptr<Callable>> &calls, const shared_ptr<Context> &context) {
			return make_shared<Array>(calls, context);
		}
	};

	extern void initialize();
}