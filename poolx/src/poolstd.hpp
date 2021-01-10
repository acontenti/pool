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

	extern bool debug;
	extern vector<shared_ptr<Object>> arguments;
	extern const shared_ptr<Class> ClassClass;
	extern const shared_ptr<Class> ObjectClass;
	extern const shared_ptr<Class> BoolClass;
	extern const shared_ptr<Class> IntegerClass;
	extern const shared_ptr<Class> DecimalClass;
	extern const shared_ptr<Class> StringClass;
	extern const shared_ptr<Class> TupleClass;
	extern const shared_ptr<Class> ArrayClass;
	extern const shared_ptr<Class> CallClass;
	extern const shared_ptr<Class> IdentityClass;
	extern const shared_ptr<Class> VariableClass;
	extern const shared_ptr<Class> BlockClass;
	extern const shared_ptr<Class> VoidClass;
	extern const shared_ptr<Class> NothingClass;
	extern const shared_ptr<Object> Void;
	extern const shared_ptr<Object> Null;

	class Context : public enable_shared_from_this<Context> {
		unordered_map<string, shared_ptr<Variable>> heap;
		const shared_ptr<Context> parent;

		Context() : Context(nullptr) {}

	public:
		static const shared_ptr<Context> global;

		explicit Context(shared_ptr<Context> parent) : parent(move(parent)) {}

		shared_ptr<Variable> find(const string &name) const;

		shared_ptr<Variable> add(const shared_ptr<Variable> &var);

		void associate(const vector<string> &params, const vector<shared_ptr<Object>> &args);

		inline auto begin() {
			return heap.begin();
		}

		inline auto cbegin() const {
			return heap.cbegin();
		}

		inline auto end() {
			return heap.end();
		}

		inline auto cend() const {
			return heap.cend();
		}

		inline auto empty() const {
			return heap.empty();
		}

		static shared_ptr<Context> create(const shared_ptr<Context> &parent) {
			return make_shared<Context>(parent);
		}
	};

	class Object : public enable_shared_from_this<Object> {
		static long ID_COUNTER;
		const long id = ID_COUNTER++;

		shared_ptr<Object> getVariableValue();

	public:
		typedef function<shared_ptr<Object>(const shared_ptr<Object> &, const shared_ptr<Object> &)> method_t;
		const shared_ptr<Class> cls;
		const shared_ptr<Context> context;

		Object(shared_ptr<Context> context, shared_ptr<Class> cls) : context(move(context)), cls(move(cls)) {}

		virtual string_view getType() const;

		inline string toString() const {
			return string(getType()) + "@" + to_string(getID());
		};

		inline long getID() const {
			return id;
		}

		template<class T>
		shared_ptr<T> as() {
			if (isVariable())
				return getVariableValue()->as<T>();
			return reinterpret_pointer_cast<T>(shared_from_this());
		}

		virtual const method_t *findMethod(const string &methodName) const;

		virtual bool isVariable() const {
			return false;
		}

		static shared_ptr<Object> create(const shared_ptr<Class> &cls, const shared_ptr<Context> &context) {
			return make_shared<Object>(context, cls);
		}
	};

	class Class : public Object {
	public:
		static long INSTANCES;
		string name;
		shared_ptr<Class> super;
		unordered_map<string, method_t> methodsMap;

		unordered_map<string, method_t> staticMethodsMap;

		Class(string name, shared_ptr<Class> super, const shared_ptr<Context> &context);

		const method_t *getMethod(const string &methodName) const;

		const method_t *findMethod(const string &methodName) const override;

		inline void addMethod(const string &methodName, const method_t &method) {
			methodsMap[methodName] = method;
		}

		inline void addStaticMethod(const string &methodName, const method_t &method) {
			staticMethodsMap[methodName] = method;
		}

		static shared_ptr<Class> create(const string &name, const shared_ptr<Class> &super, const shared_ptr<Context> &context = Context::global) {
			return make_shared<Class>(name, super, context);
		}
	};

	class Bool : public Object {
	public:
		bool value;

		Bool(bool value, const shared_ptr<Context> &context) : Object(context, BoolClass), value(value) {}

		static shared_ptr<Bool> create(const bool &value, const shared_ptr<Context> &context) {
			return make_shared<Bool>(value, context);
		}
	};

	class Integer : public Object {
	public:
		int64_t value;

		Integer(int64_t value, const shared_ptr<Context> &context) : Object(context, IntegerClass), value(value) {}

		static shared_ptr<Integer> create(const int64_t &value, const shared_ptr<Context> &context) {
			return make_shared<Integer>(value, context);
		}
	};

	class Decimal : public Object {
	public:
		double value;

		Decimal(double value, const shared_ptr<Context> &context) : Object(context, DecimalClass), value(value) {}

		static shared_ptr<Decimal> create(const double &value, const shared_ptr<Context> &context) {
			return make_shared<Decimal>(value, context);
		}
	};

	class String : public Object {
	public:
		string value;

		String(string value, const shared_ptr<Context> &context) : Object(context, StringClass), value(move(value)) {}

		static shared_ptr<String> create(const string &value, const shared_ptr<Context> &context) {
			return make_shared<String>(value, context);
		}
	};

	class Tuple : public Object {
	public:
		vector<shared_ptr<Object>> values;

		Tuple(vector<shared_ptr<Object>> values, const shared_ptr<Context> &context)
				: Object(context, TupleClass), values(move(values)) {}

		static shared_ptr<Tuple> create(const vector<shared_ptr<Object>>& values, const shared_ptr<Context> &context) {
			return make_shared<Tuple>(values, context);
		}
	};

	class Array : public Object {
	public:
		vector<shared_ptr<Object>> values;

		Array(vector<shared_ptr<Object>> values, const shared_ptr<Context> &context)
				: Object(context, ArrayClass), values(move(values)) {}

		static shared_ptr<Array> create(const vector<shared_ptr<Object>>& values, const shared_ptr<Context> &context) {
			return make_shared<Array>(values, context);
		}
	};

	class Callable : public Object {
	public:
		Callable(const shared_ptr<Context> &context, const shared_ptr<Class> &cls) : Object(context, cls) {}

		virtual shared_ptr<Object> invoke() = 0;
	};

	class Call : public Callable {
	protected:
		shared_ptr<Object> caller;
		string method;
		shared_ptr<Object> callee;
		bool prefix;
	public:

		Call(shared_ptr<Object> caller, string method, shared_ptr<Object> callee, bool prefix, const shared_ptr<Context> &context)
				: Callable(context, CallClass), caller(move(caller)), method(move(method)), callee(move(callee)),
				  prefix(prefix) {}

		shared_ptr<Object> invoke() override {
			auto first = caller, second = callee;
			if (auto call = dynamic_pointer_cast<Callable>(caller)) {
				first = call->invoke();
			}
			if (auto call = dynamic_pointer_cast<Callable>(callee)) {
				second = call->invoke();
			}
			auto function = first->findMethod(method);
			if (function) {
				return (*function)(first, second);
			} else throw execution_error("method " + method + " not found");
		}

		static shared_ptr<Call> create(const shared_ptr<Object> &caller, const string &method, const shared_ptr<Object> &callee, const bool &prefix, const shared_ptr<Context> &context) {
			return make_shared<Call>(caller, method, callee, prefix, context);
		}
	};

	class Identity : public Callable {
		shared_ptr<Object> result;
	public:
		Identity(const shared_ptr<Object> &result, const shared_ptr<Context> &context)
				: Callable(context, IdentityClass), result(result) {}

		shared_ptr<Object> invoke() override {
			if (auto call = dynamic_pointer_cast<Callable>(result)) {
				return call->invoke();
			} else return result;
		}

		static shared_ptr<Identity> create(const shared_ptr<Object> &caller, const shared_ptr<Context> &context) {
			return make_shared<Identity>(caller, context);
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

		const method_t *findMethod(const string &methodName) const override {
			const method_t *method = value->findMethod(methodName);
			if (method) {
				return method;
			} else {
				return Object::findMethod(methodName);
			}
		}

		string_view getType() const override {
			return value->getType();
		}

		bool isVariable() const override {
			return true;
		}

		Variable(string name, shared_ptr<Object> value, const shared_ptr<Context> &context, bool immutable = false)
				: Object(context, VariableClass), name(move(name)), value(move(value)), immutable(immutable) {
		}

		static shared_ptr<Variable> create(const string &name, const shared_ptr<Object> &value, const shared_ptr<Context> &context, bool immutable) {
			return make_shared<Variable>(name, value, context, immutable);
		}

		static shared_ptr<Variable> create(const string &name, const shared_ptr<Context> &context) {
			return make_shared<Variable>(name, Null, context);
		}
	};

	class Block : public Callable {
	public:
		vector<string> params;
		vector<shared_ptr<Callable>> calls;

		Block(vector<string> params, vector<shared_ptr<Callable>> calls, const shared_ptr<Context> &context)
				: Callable(context, BlockClass), params(move(params)), calls(move(calls)) {
			for (auto &param : params) {
				context->add(Variable::create(param, context));
			}
		}

		shared_ptr<Object> execute(const vector<shared_ptr<Object>> &args) {
			shared_ptr<Object> returnValue = Void;
			if (params.size() != args.size())
				return Null;
			context->associate(params, args);
			for (auto &call: calls) {
				returnValue = call->invoke();
			}
			return returnValue;
		}

		shared_ptr<Object> invoke() override {
			if (this->params.empty()) {
				execute({});
			}
			return shared_from_this();
		}

		static vector<shared_ptr<Object>> makeArgs(const shared_ptr<Object> &other, const shared_ptr<Object> &prepend = nullptr);

		static shared_ptr<Block> create(const vector<string> &params, const vector<shared_ptr<Callable>> &calls, const shared_ptr<Context> &context) {
			return make_shared<Block>(params, calls, context);
		}
	};

	extern bool initialize() noexcept;

	[[maybe_unused]]
	extern const bool initialized;
}