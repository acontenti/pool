#pragma once

#include <functional>
#include <any>
#include "callable.hpp"
#include "context.hpp"
#include "util/location.hpp"

using namespace std;

namespace pool {
	class POOL_PUBLIC Class;

	class POOL_PUBLIC Object;

	class POOL_PUBLIC Bool;

	class POOL_PUBLIC Block;

	class POOL_PUBLIC Function;

	extern POOL_PUBLIC shared_ptr<Class> ClassClass;
	extern POOL_PUBLIC shared_ptr<Class> ObjectClass;
	extern POOL_PUBLIC shared_ptr<Class> BoolClass;
	extern POOL_PUBLIC shared_ptr<Class> NumberClass;
	extern POOL_PUBLIC shared_ptr<Class> IntegerClass;
	extern POOL_PUBLIC shared_ptr<Class> DecimalClass;
	extern POOL_PUBLIC shared_ptr<Class> StringClass;
	extern POOL_PUBLIC shared_ptr<Class> ArrayClass;
	extern POOL_PUBLIC shared_ptr<Class> BlockClass;
	extern POOL_PUBLIC shared_ptr<Class> FunctionClass;
	extern POOL_PUBLIC shared_ptr<Class> VoidClass;
	extern POOL_PUBLIC shared_ptr<Class> NothingClass;
	extern POOL_PUBLIC shared_ptr<Object> Void;
	extern POOL_PUBLIC shared_ptr<Object> Null;
	extern POOL_PUBLIC shared_ptr<Bool> True;
	extern POOL_PUBLIC shared_ptr<Bool> False;

	class POOL_PUBLIC Object : public enable_shared_from_this<Object> {
	protected:
		const size_t id;
		const shared_ptr<Class> cls;
	public:
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &data) {
			return make_shared<Object>(context, any_cast<shared_ptr<Class>>(data));
		};
		const shared_ptr<Context> context;

		Object(const shared_ptr<Context> &context, shared_ptr<Class> cls);

		shared_ptr<Class> getClass() const;

		string getRepr() const;

		template<class T, typename enable_if<is_base_of<Object, T>::value, int>::type = 0>
		inline shared_ptr<T> as() const {
			return const_pointer_cast<T>(reinterpret_pointer_cast<const T>(shared_from_this()));
		}

		shared_ptr<Function> findMethod(const string &methodName) const;

		virtual shared_ptr<Variable> find(const string &name) const;

		shared_ptr<Variable> findLocal(const string &id) const;

		void remove(const string &name);

		bool instanceOf(const shared_ptr<Class> &_class) const;
	};

	class POOL_PUBLIC Class : public Object {
	public:
		using creator_t = function<shared_ptr<Object>(const shared_ptr<Context> &context, const any &data)>;
		struct ClassData {
			const Class::creator_t &creator;
			const string &name;
			const shared_ptr<Class> &super;
		};
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &data) {
			const auto &classData = any_cast<Class::ClassData>(data);
			return make_shared<Class>(context, classData.creator, classData.name, classData.super);
		};
		size_t instances = 0;
		string name;
		shared_ptr<Class> super;
		creator_t creator;

		Class(const shared_ptr<Context> &context, creator_t creator, string name, shared_ptr<Class> super);

		shared_ptr<Variable> find(const string &id) const override;

		shared_ptr<Variable> findInClass(const string &id) const;

		shared_ptr<Object> newInstance(const shared_ptr<Context> &parent, const Location &location, const vector<shared_ptr<Object>> &other, const any &data) const;

		shared_ptr<Class> extend(const creator_t &creator, const string &className, const shared_ptr<Block> &block, const Location &location) const;

		bool subclassOf(const shared_ptr<Class> &other) const;

		bool superclassOf(const shared_ptr<Class> &other) const;
	};

	class POOL_PUBLIC Bool : public Object {
	public:
		bool value;
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &data) {
			return make_shared<Bool>(context, any_cast<bool>(data));
		};

		Bool(const shared_ptr<Context> &context, bool value);
	};

	class POOL_PUBLIC Integer : public Object {
	public:
		long long int value;
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &_value) {
			return make_shared<Integer>(context, any_cast<long long int>(_value));
		};

		Integer(const shared_ptr<Context> &context, long long int value);

		static shared_ptr<Object> newInstance(const shared_ptr<Context> &context, const Location &location, const long long int &value) {
			return IntegerClass->newInstance(context, location, {}, value);
		}
	};

	class POOL_PUBLIC Decimal : public Object {
	public:
		long double value;
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &data) {
			return make_shared<Decimal>(context, any_cast<long double>(data));
		};

		Decimal(const shared_ptr<Context> &context, long double value);

		static shared_ptr<Object> newInstance(const shared_ptr<Context> &context, const Location &location, const long double &value) {
			return DecimalClass->newInstance(context, location, {}, value);
		}
	};

	class POOL_PUBLIC String : public Object {
	public:
		string value;
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &data) {
			return make_shared<String>(context, any_cast<string>(data));
		};

		String(const shared_ptr<Context> &context, string value);

		static shared_ptr<Object> newInstance(const shared_ptr<Context> &context, const Location &location, const string &value) {
			return StringClass->newInstance(context, location, {}, value);
		}
	};

	class POOL_PUBLIC Executable : public Object {
	public:
		Executable(const shared_ptr<Context> &context, const shared_ptr<Class> &cls);

		virtual shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) = 0;
	};

	class POOL_PUBLIC Function : public Executable {
	public:
		struct Param {
			string id;
			bool rest;
			shared_ptr<Class> type;

			Param(string id, const bool &rest = false) : id(move(id)), type(nullptr), rest(rest) {}

			Param(string id, shared_ptr<Class> type) : id(move(id)), type(move(type)), rest(false) {}
		};

		vector<Param> params;

		Function(const shared_ptr<Context> &context, const vector<Param> &params);
	};

	class POOL_PUBLIC NativeFunction : public Function {
	public:
		using method_t = function<shared_ptr<Object>(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location)>;
	private:
		method_t code;
	public:
		NativeFunction(const shared_ptr<Context> &context, const vector<Param> &params, method_t code);

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) override;

		static shared_ptr<NativeFunction> newInstance(const vector<Param> &params, const method_t &code);
	};

	class POOL_PUBLIC CodeFunction : public Function {
		vector<shared_ptr<Callable>> calls;
	public:
		struct CodeFunctionData {
			const vector<Param> &params;
			const vector<shared_ptr<Callable>> &calls;
		};
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &data) {
			const auto &functionData = any_cast<CodeFunctionData>(data);
			return make_shared<CodeFunction>(context, functionData.params, functionData.calls);
		};

		CodeFunction(const shared_ptr<Context> &context, const vector<Param> &params, const vector<shared_ptr<Callable>> &calls);

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) override;

		static shared_ptr<Object> newInstance(const shared_ptr<Context> &context, const Location &location, const vector<Param> &params, const vector<shared_ptr<Callable>> &calls);
	};

	class POOL_PUBLIC Block : public Executable {
	public:
		vector<shared_ptr<Callable>> calls;
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &data) {
			return make_shared<Block>(context, any_cast<vector<shared_ptr<Callable>>>(data));
		};

		Block(const shared_ptr<Context> &context, const vector<shared_ptr<Callable>> &calls);

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) override;

		shared_ptr<Object> execute(const Location &location);

		static shared_ptr<Object> newInstance(const shared_ptr<Context> &context, const Location &location, const vector<shared_ptr<Callable>> &calls);
	};

	class POOL_PUBLIC Array : public Object {
	public:
		vector<shared_ptr<Object>> values;
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &data) {
			return make_shared<Array>(context, any_cast<vector<shared_ptr<Object>>>(data));
		};

		explicit Array(const shared_ptr<Context> &context, const vector<shared_ptr<Object>> &values);

		static shared_ptr<Object> newInstance(const shared_ptr<Context> &context, const Location &location, const vector<shared_ptr<Object>> &values);
	};
}