#pragma once

#include <sstream>
#include <utility>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include "util.hpp"

using namespace std;

namespace pool {
	class Class;

	class Variable;

	class Object;

	class Block;

	class Executable;

	using method_t = function<shared_ptr<Object>(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other)>;

	extern bool debug;
	extern vector<shared_ptr<Object>> arguments;
	extern const shared_ptr<Class> ClassClass;
	extern const shared_ptr<Class> ObjectClass;
	extern const shared_ptr<Class> BoolClass;
	extern const shared_ptr<Class> IntegerClass;
	extern const shared_ptr<Class> DecimalClass;
	extern const shared_ptr<Class> StringClass;
	extern const shared_ptr<Class> ArrayClass;
	extern const shared_ptr<Class> VariableClass;
	extern const shared_ptr<Class> BlockClass;
	extern const shared_ptr<Class> FunClass;
	extern const shared_ptr<Class> VoidClass;
	extern const shared_ptr<Class> NothingClass;
	extern const shared_ptr<Object> Void;
	extern const shared_ptr<Object> Null;

	class Context : public enable_shared_from_this<Context> {
		unordered_map<string, shared_ptr<Object>> heap;
		const shared_ptr<Context> parent;

		Context() : Context(nullptr) {}

	public:
		static const shared_ptr<Context> global;

		explicit Context(shared_ptr<Context> parent) : parent(move(parent)) {}

		shared_ptr<Object> find(const string &name) const;

		shared_ptr<Executable> findMethod(const string &name) const;

		shared_ptr<Object> add(const string &name);

		shared_ptr<Object> add(const string &name, const shared_ptr<Object> &var);

		void set(const string &name, const shared_ptr<Object> &value);

		void associate(const vector<string> &params, const vector<shared_ptr<Object>> &args);

		string toString() const;

		inline auto begin() const {
			return heap.begin();
		}

		inline auto end() const {
			return heap.end();
		}

		inline auto empty() const {
			return heap.empty();
		}

		static shared_ptr<Context> create(const shared_ptr<Context> &parent) {
			return make_shared<Context>(parent);
		}
	};

	class Object : public enable_shared_from_this<Object> {
		shared_ptr<Object> getVariableValue();

	public:
		const size_t id;
		const shared_ptr<Class> cls;
		const shared_ptr<Context> context;

		Object(shared_ptr<Context> context, shared_ptr<Class> cls);

		virtual string_view getType();

		inline string toString() {
			return string(getType()) + "@" + to_string(id);
		};

		template<class T>
		shared_ptr<T> as() {
			if (isVariable())
				return getVariableValue()->as<T>();
			return reinterpret_pointer_cast<T>(shared_from_this());
		}

		virtual shared_ptr<Executable> findMethod(const string &methodName) const;

		virtual shared_ptr<Object> find(const string &name) const;

		virtual bool isVariable() const {
			return false;
		}

		static shared_ptr<Object> create(const shared_ptr<Class> &cls, const shared_ptr<Context> &context, const vector<shared_ptr<Object>> &other);
	};

	class Class : public Object {
	public:
		constexpr static const string_view TYPE = "Class";
		static std::size_t COUNTER;
		size_t instances = 0;
		string name;
		shared_ptr<Class> super;

		Class(string name, shared_ptr<Class> super, const shared_ptr<Context> &context);

		shared_ptr<Executable> getMethod(const string &methodName) const;

		shared_ptr<Executable> findMethod(const string &methodName) const override;

		shared_ptr<Object> find(const string &id) const override {
			if (auto local = findLocal(id)) {
				return local;
			}
			if (cls) {
				return cls->find(id);
			}
			return nullptr;
		}

		shared_ptr<Object> findLocal(const string &id) const {
			if (auto local = context->find(id)) {
				return local;
			}
			if (super) {
				return super->findLocal(id);
			}
			return nullptr;
		};

		void addMethod(const string &methodName, const shared_ptr<Executable> &method);

		void addMethod(const string &methodName, const method_t &method);

		shared_ptr<Object> newInstance(const vector<shared_ptr<Object>> &other);

		shared_ptr<Class> extend(const shared_ptr<Block> &other);

		static shared_ptr<Class> create(const string &name, const shared_ptr<Class> &super, const shared_ptr<Context> &context = Context::global) {
			return make_shared<Class>(name, super, context);
		}
	};

	class Bool : public Object {
	public:
		bool value;
		constexpr static const string_view TYPE = "Bool";

		Bool(bool value, const shared_ptr<Context> &context) : Object(context, BoolClass), value(value) {}

		static shared_ptr<Bool> create(const bool &value, const shared_ptr<Context> &context) {
			return make_shared<Bool>(value, context);
		}
	};

	class Integer : public Object {
	public:
		int64_t value;
		constexpr static const string_view TYPE = "Integer";

		Integer(int64_t value, const shared_ptr<Context> &context) : Object(context, IntegerClass), value(value) {}

		static shared_ptr<Integer> create(const int64_t &value, const shared_ptr<Context> &context) {
			return make_shared<Integer>(value, context);
		}
	};

	class Decimal : public Object {
	public:
		double value;
		constexpr static const string_view TYPE = "Decimal";

		Decimal(double value, const shared_ptr<Context> &context) : Object(context, DecimalClass), value(value) {}

		static shared_ptr<Decimal> create(const double &value, const shared_ptr<Context> &context) {
			return make_shared<Decimal>(value, context);
		}
	};

	class String : public Object {
	public:
		constexpr static const string_view TYPE = "String";
		string value;

		String(string value, const shared_ptr<Context> &context) : Object(context, StringClass), value(move(value)) {}

		static shared_ptr<String> create(const string &value, const shared_ptr<Context> &context) {
			return make_shared<String>(value, context);
		}

	};

	class Callable {
	public:
		virtual shared_ptr<Object> invoke() = 0;
	};

	class Access : public Callable {
		shared_ptr<Callable> caller;
		string id;
	public:

		Access(shared_ptr<Callable> caller, string id) : caller(move(caller)), id(move(id)) {}

		shared_ptr<Object> invoke() override {
			auto ptr = caller->invoke();
			auto result = ptr->find(id);
			return result ? result : ptr->context->add(id);
		}

		static shared_ptr<Access> create(const shared_ptr<Callable> &caller, const string &id) {
			return make_shared<Access>(caller, id);
		}
	};

	class Invocation : public Callable {
		shared_ptr<Callable> self;
		shared_ptr<Callable> caller;
		vector<shared_ptr<Callable>> callee;
	public:

		Invocation(const shared_ptr<Callable> &self, shared_ptr<Callable> caller, const vector<shared_ptr<Callable>> &callee)
				: self(self), caller(move(caller)), callee(callee) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Invocation> create(const shared_ptr<Callable> &self, const shared_ptr<Callable> &caller, const vector<shared_ptr<Callable>> &callee) {
			return make_shared<Invocation>(self, caller, callee);
		}
	};

	class Identity : public Callable {
		shared_ptr<Object> object;
	public:
		explicit Identity(const shared_ptr<Object> &object) : object(object) {}

		shared_ptr<Object> invoke() override {
			return object;
		};

		static shared_ptr<Callable> create(const shared_ptr<Object> &caller) {
			return make_shared<Identity>(caller);
		}
	};


	class Variable : public Object {
		shared_ptr<Object> value;
		bool immutable = false;
	public:
		const string name;

		bool isImmutable() const {
			return immutable;
		}

		void setImmutable(bool _immutable) {
			Variable::immutable = _immutable;
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
			if (auto method = value->findMethod(methodName)) {
				return method;
			} else return Object::findMethod(methodName);
		}

		shared_ptr<Object> find(const string &id) const override {
			if (auto result = value->find(id)) {
				return result;
			} else return Object::find(id);
		}

		string_view getType() override {
			return value->getType();
		}

		bool isVariable() const override {
			return true;
		}

		Variable(string name, shared_ptr<Object> value, const shared_ptr<Context> &context, bool immutable)
				: Object(context, VariableClass), name(move(name)), value(move(value)), immutable(immutable) {
		}

		static shared_ptr<Variable> create(const string &name, const shared_ptr<Context> &context) {
			return make_shared<Variable>(name, Null, context, false);
		}
	};

	class Executable : public Object {
	public:
		vector<string> params;

		Executable(const vector<string> &params, const shared_ptr<Context> &context, const shared_ptr<Class> &cls)
				: Object(context, cls), params(params) {}

		virtual shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) = 0;
	};

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
		vector<shared_ptr<Callable>> calls;

		Block(vector<shared_ptr<Callable>> calls, const shared_ptr<Context> &context)
				: Executable({}, context, BlockClass), calls(move(calls)) {}

		shared_ptr<Object> invoke() override {
			for (auto &call: calls) {
				call->invoke();
			}
			return shared_from_this();
		}

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) override {
			return invoke();
		}

		static shared_ptr<Block> create(const vector<shared_ptr<Callable>> &calls, const shared_ptr<Context> &context) {
			return make_shared<Block>(calls, context);
		}
	};

	class Array : public Callable, public Executable {
	public:
		vector<shared_ptr<Callable>> calls;
		vector<shared_ptr<Object>> values;

		Array(vector<shared_ptr<Callable>> calls, const shared_ptr<Context> &context)
				: Executable({"index"}, context, ArrayClass), calls(move(calls)) {}

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) override;

		shared_ptr<Object> invoke() override;

		static shared_ptr<Array> create(const vector<shared_ptr<Callable>> &calls, const shared_ptr<Context> &context) {
			return make_shared<Array>(calls, context);
		}
	};

	extern bool initialize() noexcept;

	[[maybe_unused]]
	extern const bool initialized;
}