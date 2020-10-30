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

	class Context : enable_shared_from_this<Context> {
		unordered_map<string, shared_ptr<Variable>> heap;
		const shared_ptr<Context> parent;

		Context() : Context(nullptr) {}

	public:
		static const shared_ptr<Context> global;

		explicit Context(shared_ptr<Context> parent) : parent(move(parent)) {}

		shared_ptr<Variable> find(const string &name) const;

		inline void add(const string &name, const shared_ptr<Variable> &var) {
			heap.emplace(name, var);
		}

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

		virtual string_view getType() const;;

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
	};

	class Class : public Object {
	public:
		string name;
		shared_ptr<Class> super;
		unordered_map<string, method_t> methodsMap;

		Class(string name, shared_ptr<Class> super, const shared_ptr<Context> &context = Context::global)
				: Object(context, ClassClass), name(move(name)), super(move(super)) {
		}

		const method_t *findMethod(const string &methodName) const override;

		inline void addMethod(const string &methodName, const method_t &method) {
			methodsMap[methodName] = method;
		}
	};

	class Bool : public Object {
	public:
		bool value;

		explicit Bool(bool value, const shared_ptr<Context> &context) : Object(context, BoolClass), value(value) {}
	};

	class Integer : public Object {
	public:
		int64_t value;

		explicit Integer(int64_t value, const shared_ptr<Context> &context)
				: Object(context, IntegerClass), value(value) {
		}
	};

	class Decimal : public Object {
	public:
		double value;

		explicit Decimal(double value, const shared_ptr<Context> &context)
				: Object(context, DecimalClass), value(value) {
		}
	};

	class String : public Object {
	public:
		string value;

		explicit String(string value, const shared_ptr<Context> &context)
				: Object(context, StringClass), value(move(value)) {
		}
	};

	class Tuple : public Object {
	public:
		vector<shared_ptr<Object>> values;
		static const shared_ptr<Tuple> Empty;

		explicit Tuple(vector<shared_ptr<Object>> values, const shared_ptr<Context> &context)
				: Object(context, TupleClass), values(move(values)) {}
	};

	class Array : public Object {
	public:
		vector<shared_ptr<Object>> values;

		explicit Array(vector<shared_ptr<Object>> values, const shared_ptr<Context> &context)
				: Object(context, ArrayClass), values(move(values)) {}
	};

	class Call : public Object {
	public:
		shared_ptr<Object> caller;
		string method;
		shared_ptr<Object> callee;

		static const shared_ptr<Call> Empty;

		class Identity : public Object {
		public:
			shared_ptr<Object> result;

			Identity(shared_ptr<Object> result, const shared_ptr<Context> &context)
					: Object(context, IdentityClass), result(move(result)) {
			}
		};

		Call(shared_ptr<Object> caller, string method, shared_ptr<Object> callee, const shared_ptr<Context> &context)
				: Object(context, CallClass), caller(move(caller)), method(move(method)), callee(move(callee)) {}

		shared_ptr<Object> invoke() const {
			auto first = caller, second = callee;
			if (auto call = dynamic_pointer_cast<Call>(caller)) {
				first = call->invoke();
			}
			if (auto call = dynamic_pointer_cast<Call>(callee)) {
				second = call->invoke();
			}
			auto function = first->findMethod(method);
			if (function) {
				return (*function)(first, second);
			} else throw execution_error(__FILE__, __LINE__, "method " + method + " not found");
		}

		static shared_ptr<Call> Identity(const shared_ptr<Object> &result, const shared_ptr<Context> &context) {
			return make_shared<Call>(make_shared<class Identity>(result, context), "", Void, context);
		}
	};

	class Variable : public Object {
	public:
		string name;
		shared_ptr<Object> value;

		const method_t *findMethod(const string &methodName) const override {
			const method_t *method = value->findMethod(methodName);
			if (method) {
				return method;
			} else {
				return Object::findMethod(methodName);
			}
		}

		Variable(string name, shared_ptr<Object> value, const shared_ptr<Context> &context)
				: Object(context, VariableClass), name(move(name)), value(move(value)) {
		}

		string_view getType() const override {
			return value->getType();
		}

		bool isVariable() const override {
			return true;
		}

		Variable(const string &name, const shared_ptr<Context> &context) : Variable(name, Null, context) {}
	};

	class Block : public Object {
	public:
		vector<string> params;
		vector<shared_ptr<Call>> calls;
		static const shared_ptr<Block> Empty;

		Block(vector<string> params, vector<shared_ptr<Call>> calls, const shared_ptr<Context> &context)
				: Object(context, BlockClass), params(move(params)), calls(move(calls)) {
			for (auto &param : params) {
				context->add(param, make_shared<Variable>(param, context));
			}
			if (this->params.empty() && this->context != Context::global) {
				execute({});
			}
		}

		shared_ptr<Object> execute(const vector<shared_ptr<Object>> &args) {
			shared_ptr<Object> returnValue = Void;
			context->associate(params, args);
			for (auto &call: calls) {
				returnValue = call->invoke();
			}
			return returnValue;
		}
	};

	extern bool initialize() noexcept;

	[[maybe_unused]]
	extern const bool initialized;
}