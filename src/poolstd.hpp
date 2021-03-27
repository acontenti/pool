#pragma once

#include <functional>
#include "callable.hpp"
#include "context.hpp"
#include "parser.hpp"

using namespace std;

namespace pool {
	using any = antlrcpp::Any;

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

		Object(const shared_ptr<Context> &context, shared_ptr<Class> cls);

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

		virtual shared_ptr<Variable> find(const string &name) const;

		virtual shared_ptr<Variable> findLocal(const string &id) const;

		virtual bool isVariable() const {
			return false;
		}
	};

	template<>
	shared_ptr<Variable> Object::as<Variable>();

	class Class : public Object {
	public:
		using creator_t = function<shared_ptr<Object>(const shared_ptr<Context> &context, const any &a1, const any &a2, const any &a3)>;

		constexpr static const string_view TYPE = "Class";
		size_t instances = 0;
		string name;
		shared_ptr<Class> super;
		creator_t creator;

		Class(const shared_ptr<Context> &context, creator_t creator, string name, shared_ptr<Class> super);

		shared_ptr<Variable> find(const string &id) const override {
			if (auto local = findInClass(id)) {
				return local;
			}
			if (cls) {
				return cls->find(id);
			}
			return nullptr;
		}

		shared_ptr<Variable> findInClass(const string &id) const {
			if (auto local = context->findLocal(id)) {
				return local;
			}
			if (super) {
				return super->findInClass(id);
			}
			return nullptr;
		};

		shared_ptr<Object> newInstance(const shared_ptr<Context> &context, const vector<shared_ptr<Object>> &other = {}, const any &a1 = nullptr, const any &a2 = nullptr, const any &a3 = nullptr, bool createContext = true) const;

		shared_ptr<Class> extend(const creator_t &creator, const string &className, const shared_ptr<Block> &other = nullptr);
	};

	class Bool : public Object {
	public:
		bool value;
		constexpr static const string_view TYPE = "Bool";

		Bool(const shared_ptr<Context> &context, bool value) : Object(context, BoolClass), value(value) {}
	};

	class Integer : public Object {
	public:
		long long int value;
		constexpr static const string_view TYPE = "Integer";

		Integer(long long int value, const shared_ptr<Context> &context)
				: Object(context, IntegerClass), value(value) {}
	};

	class Decimal : public Object {
	public:
		long double value;
		constexpr static const string_view TYPE = "Decimal";

		Decimal(double value, const shared_ptr<Context> &context) : Object(context, DecimalClass), value(value) {}
	};

	class String : public Object {
	public:
		constexpr static const string_view TYPE = "String";
		string value;

		String(const shared_ptr<Context> &context, string value) : Object(context, StringClass), value(move(value)) {}
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
			immutable = _immutable;
		}

		const shared_ptr<Object> &getValue() const {
			return value;
		}

		void setValue(const shared_ptr<Object> &val);

		shared_ptr<Executable> findMethod(const string &methodName) const override {
			return value->findMethod(methodName);
		}

		shared_ptr<Variable> find(const string &id) const override {
			return value->find(id);
		}

		shared_ptr<Variable> findLocal(const string &id) const override {
			return value->findLocal(id);
		}


		string_view getType() override {
			return value->getType();
		}

		bool isVariable() const override {
			return true;
		}

		Variable(const shared_ptr<Context> &context, string name, shared_ptr<Object> value, bool immutable)
				: Object(context, nullptr), name(move(name)), value(move(value)), immutable(immutable) {
		}
	};

	class Executable : public Object {
	public:
		struct Param {
			string id;
			bool rest;

			Param(string id, bool rest = false) : id(std::move(id)), rest(rest) {}
		};

		vector<Param> params;

		Executable(const vector<Param> &params, const shared_ptr<Context> &context, const shared_ptr<Class> &cls)
				: Object(context, cls), params(params) {}

		virtual shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) = 0;
	};

	using method_t = function<shared_ptr<Object>(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other)>;

	class NativeFun : public Executable {
	public:
		method_t code;

		NativeFun(const vector<Param> &params, method_t code, const shared_ptr<Context> &context)
				: Executable(params, context, FunClass), code(move(code)) {}

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) override;

		static shared_ptr<NativeFun> create(const vector<Param> &params, const method_t &code) {
			return make_shared<NativeFun>(params, code, Context::global);
		}
	};

	class Fun : public Executable {
	public:
		constexpr static const string_view TYPE = "Fun";
		vector<shared_ptr<Callable>> calls;

		Fun(const vector<Param> &params, vector<shared_ptr<Callable>> calls, const shared_ptr<Context> &context)
				: Executable(params, context, FunClass), calls(move(calls)) {}

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) override;

		void associateContext(const vector<shared_ptr<pool::Object>> &args);
	};

	class Block : public Executable {
	public:
		constexpr static const string_view TYPE = "Block";
		vector<shared_ptr<Callable>> calls;

		Block(vector<shared_ptr<Callable>> calls, const shared_ptr<Context> &context)
				: Executable({}, context, BlockClass), calls(move(calls)) {}

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other) override;
	};

	class Array : public Object {
	public:
		constexpr static const string_view TYPE = "Array";
		vector<shared_ptr<Object>> values;

		explicit Array(const shared_ptr<Context> &context) : Object(context, ArrayClass) {}
	};

	extern void initialize();
}