#pragma once

#include <functional>
#include <any>
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
		shared_ptr<Object> getVariableValue() const;

	public:
		constexpr static const string_view TYPE = "Object";
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &cls, const any &, const any &) {
			return make_shared<Object>(context, any_cast<shared_ptr<Class>>(cls));
		};
		const size_t id;
		const shared_ptr<Class> cls;
		const shared_ptr<Context> context;

		Object(const shared_ptr<Context> &context, shared_ptr<Class> cls);

		virtual string_view getType();

		string toString();

		template<class T, typename enable_if<is_base_of<Object, T>::value, int>::type = 0>
		shared_ptr<T> as() const {
			if (isVariable())
				return const_pointer_cast<T>(reinterpret_pointer_cast<const T>(getVariableValue()));
			return const_pointer_cast<T>(reinterpret_pointer_cast<const T>(shared_from_this()));
		}

		virtual shared_ptr<Executable> findMethod(const string &methodName) const;

		virtual shared_ptr<Variable> find(const string &name) const;

		virtual shared_ptr<Variable> findLocal(const string &id) const;

		virtual bool isVariable() const;

		void remove(const string &name);

		bool instanceOf(const shared_ptr<Class> &_class);
	};

	class Class : public Object {
	public:
		using creator_t = function<shared_ptr<Object>(const shared_ptr<Context> &context, const any &p1, const any &p2, const any &p3)>;

		constexpr static const string_view TYPE = "Class";
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &creator, const any &name, const any &super) {
			return make_shared<Class>(context, any_cast<Class::creator_t>(creator), any_cast<string>(name), any_cast<shared_ptr<Class>>(super));
		};
		size_t instances = 0;
		string name;
		shared_ptr<Class> super;
		creator_t creator;

		Class(const shared_ptr<Context> &context, creator_t creator, string name, shared_ptr<Class> super);

		shared_ptr<Variable> find(const string &id) const override;

		shared_ptr<Variable> findInClass(const string &id) const;

		shared_ptr<Object> newInstance(const shared_ptr<Context> &parent, const Location &location, const vector<shared_ptr<Object>> &other = {}, const any &a1 = nullptr, const any &a2 = nullptr, const any &a3 = nullptr) const;

		shared_ptr<Class> extend(const creator_t &creator, const string &className, const shared_ptr<Block> &other, const Location &location) const;

		bool subclassOf(const shared_ptr<Class> &_class) const;

		bool superclassOf(const shared_ptr<Class> &_class) const;
	};

	class Variable : public Object {
		shared_ptr<Object> value;
		bool immutable = false;
	public:
		const string name;

		bool isImmutable() const;

		void setImmutable(bool _immutable);

		const shared_ptr<Object> &getValue() const;

		void setValue(const shared_ptr<Object> &val);

		shared_ptr<Executable> findMethod(const string &methodName) const override;

		shared_ptr<Variable> find(const string &id) const override;

		shared_ptr<Variable> findLocal(const string &id) const override;

		string_view getType() override;

		bool isVariable() const override;

		Variable(const shared_ptr<Context> &context, string name, shared_ptr<Object> value, bool immutable);
	};

	template<>
	shared_ptr<Variable> Object::as<Variable>() const;

	class Bool : public Object {
	public:
		bool value;
		constexpr static const string_view TYPE = "Bool";
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &_value, const any &, const any &) {
			return make_shared<Bool>(context, any_cast<bool>(_value));
		};

		Bool(const shared_ptr<Context> &context, bool value);
	};

	class Integer : public Object {
	public:
		long long int value;
		constexpr static const string_view TYPE = "Integer";
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &_value, const any &, const any &) {
			return make_shared<Integer>(context, any_cast<long long int>(_value));
		};

		Integer(const shared_ptr<Context> &context, long long int value);

		static shared_ptr<Object> newInstance(const shared_ptr<Context> &context, const Location &location, const long long int &value) {
			return IntegerClass->newInstance(context, location, {}, value);
		}
	};

	class Decimal : public Object {
	public:
		long double value;
		constexpr static const string_view TYPE = "Decimal";
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &_value, const any &, const any &) {
			return make_shared<Decimal>(context, any_cast<long double>(_value));
		};

		Decimal(const shared_ptr<Context> &context, long double value);

		static shared_ptr<Object> newInstance(const shared_ptr<Context> &context, const Location &location, const long double &value) {
			return DecimalClass->newInstance(context, location, {}, value);
		}
	};

	class String : public Object {
	public:
		string value;
		constexpr static const string_view TYPE = "String";
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &_value, const any &, const any &) {
			return make_shared<String>(context, any_cast<string>(_value));
		};

		String(const shared_ptr<Context> &context, string value);

		static shared_ptr<Object> newInstance(const shared_ptr<Context> &context, const Location &location, const string &value) {
			return StringClass->newInstance(context, location, {}, value);
		}
	};

	class Executable : public Object {
	public:
		Executable(const shared_ptr<Context> &context, const shared_ptr<Class> &cls);

		virtual shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) = 0;
	};

	class Function : public Executable {
	public:
		struct Param {
			string id;
			bool rest;

			Param(string id, bool rest = false) : id(move(id)), rest(rest) {}
		};

		vector<Param> params;

		Function(const shared_ptr<Context> &context, const vector<Param> &params);

		constexpr static const string_view TYPE = "Fun";
	};

	class Fun : public Function {
		vector<PoolParser::StatementContext *> calls;
	public:
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &_params, const any &_calls, const any &) {
			return make_shared<Fun>(context, any_cast<vector<Param>>(_params), any_cast<vector<PoolParser::StatementContext *>>(_calls));
		};

		Fun(const shared_ptr<Context> &context, const vector<Param> &params, vector<PoolParser::StatementContext *> calls);

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) override;

		static shared_ptr<Object> newInstance(const shared_ptr<Context> &context, const Location &location, const vector<Param> &params, const vector<PoolParser::StatementContext *> &calls) {
			return FunClass->newInstance(context, location, {}, params, calls);
		}
	};

	class NativeFun : public Function {
	public:
		using method_t = function<shared_ptr<Object>(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location)>;
	private:
		method_t code;
	public:
		NativeFun(const shared_ptr<Context> &context, const vector<Param> &params, method_t code);

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) override;

		static shared_ptr<NativeFun> newInstance(const vector<Param> &params, const method_t &code);
	};

	class Block : public Executable {
	public:
		vector<PoolParser::StatementContext *> calls;
		constexpr static const string_view TYPE = "Block";
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &_calls, const any &, const any &) {
			return make_shared<Block>(context, any_cast<vector<PoolParser::StatementContext *>>(_calls));
		};

		Block(const shared_ptr<Context> &context, vector<PoolParser::StatementContext *> calls);

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) override;

		shared_ptr<Object> execute(const Location &location);

		static shared_ptr<Object> newInstance(const shared_ptr<Context> &context, const Location &location, const vector<PoolParser::StatementContext *> &calls) {
			return BlockClass->newInstance(context, location, {}, calls);
		}
	};

	class Array : public Object {
	public:
		vector<shared_ptr<Object>> values;
		constexpr static const string_view TYPE = "Array";
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &, const any &, const any &) {
			return make_shared<Array>(context);
		};

		explicit Array(const shared_ptr<Context> &context);
	};
}