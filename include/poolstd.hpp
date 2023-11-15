#pragma once

#include <util/macro.hpp>
#include <unordered_map>
#include <string>
#include <cstring>

namespace pool {
	struct Context;
	struct Object;
	struct Class;
	struct String;
	struct Bool;
	struct Integer;
	struct Decimal;
	struct Module;
	struct Function;
	struct Array;

	POOL_PUBLIC EXTERN Class *$ObjectClass;
	POOL_PUBLIC EXTERN Class *$ClassClass;
	POOL_PUBLIC EXTERN Class *$BooleanClass;
	POOL_PUBLIC EXTERN Class *$IntegerClass;
	POOL_PUBLIC EXTERN Class *$DecimalClass;
	POOL_PUBLIC EXTERN Class *$StringClass;
	POOL_PUBLIC EXTERN Class *$ModuleClass;
	POOL_PUBLIC EXTERN Class *$FunctionClass;
	POOL_PUBLIC EXTERN Class *$ArrayClass;
	POOL_PUBLIC EXTERN Class *$NothingClass;
	POOL_PUBLIC EXTERN Bool *$True;
	POOL_PUBLIC EXTERN Bool *$False;
	POOL_PUBLIC EXTERN Object *$Null;

	class POOL_PUBLIC Context {
		std::unordered_map<std::string, std::pair<Object *, bool>> heap;
		Context *parent;
	public:
		Context() : parent(nullptr) {}

		explicit Context(Context *parent) : parent(parent) {}

		[[nodiscard]] Object *get(const std::string &name) const;

		Object *set(const std::string &name, Object *value, bool constant);

		void del(const std::string &name);

		[[nodiscard]] std::string toString() const;

		[[nodiscard]] Object *findLocal(const std::string &name) const;

		[[nodiscard]] inline auto begin() const {
			return heap.begin();
		}

		[[nodiscard]] inline auto end() const {
			return heap.end();
		}

		POOL_PUBLIC static Context *global;
	};

	class POOL_PUBLIC Object {
	public:
		const char *id;
		Class *cls;
		Context *context;
	protected:
		Object(Class *cls, Context *context);

	public:
		[[nodiscard]] Class *getClass() const;

		[[nodiscard]] std::string getDefaultRepr() const;

		[[nodiscard]] String *getRepr();

		[[nodiscard]] String *getContextInfo() const;

		[[nodiscard]] Function *findMethod(const std::string &name) const;

		[[nodiscard]] Object *find(const std::string &name) const;

		[[nodiscard]] Object *findLocal(const std::string &name) const;

		[[nodiscard]] Bool *instanceOf(Class *_cls) const;

		[[nodiscard]] String *toString();

		bool operator==(const Object *rhs) const;

		static Object *CREATOR(Context *_context, void *data);
	};

	class POOL_PUBLIC Class : public Object {
	public:
		using creator_t = Object *(*)(Context *, void *data);
		const char *name;
		const Class *super;
		creator_t creator;
	private:
		explicit Class(const char *name, const Class *super, creator_t creator, Context *context)
				: Object($ClassClass, context), name(name), super(super), creator(creator) {}

	public:
		struct ClassData {
			const char *name;
			const Class *super;
			creator_t creator;
		};

		[[nodiscard]] Object *find(const std::string &id) const;

		[[nodiscard]] Object *findInClass(const std::string &id) const;

		Object *newInstance(Context *parent, void *data, Object **args, size_t argc) const;

		Object *newInstance(Context *parent, void *data) const {
			return newInstance(parent, data, nullptr, 0);
		}

		Class *extend(const char *className, creator_t _creator, Function *block) const;

		Class *extend(const char *className, creator_t _creator) const {
			return extend(className, _creator, nullptr);
		}

		Class *extend(Class *newClass, Function *block) const;

		bool subclassOf(const Class *_cls) const;

		bool superclassOf(const Class *_cls) const;

		[[nodiscard]] String *getRepr();

		bool operator==(const Class *rhs) const;

		static Object *CREATOR(Context *_context, void *data);

		static Class *create(const char *className, const Class *super, const creator_t &creator, Context *context);
	};

	class POOL_PUBLIC Bool : public Object {
	public:
		bool value;
	private:
		explicit Bool(bool value, Context *context);

	public:
		static Object *CREATOR(Context *_context, void *data);
	};

	struct POOL_PUBLIC Integer : public Object {
	public:
		long long int value;
	private:
		explicit Integer(long long int value, Context *context);

	public:
		static Object *CREATOR(Context *_context, void *data);

		static Integer *create(long long int value, Context *context);
	};

	struct POOL_PUBLIC Decimal : public Object {
	public:
		long double value;
	private:
		explicit Decimal(long double value, Context *context);

	public:
		static Object *CREATOR(Context *_context, void *data);

		static Decimal *create(long double value, Context *context);
	};

	struct POOL_PUBLIC String : public Object {
	public:
		const char *value;
		size_t length;
	private:
		explicit String(const char *value, Context *context);

	public:
		static Object *CREATOR(Context *_context, void *data);

		static String *create(const char *value, Context *context);
	};

	struct POOL_PUBLIC Module : public Object {
	public:
		using function_t = Object *(*)(Object *);
		const char *path;
		function_t function;
		struct ModuleData {
			const char *path;
			function_t function;
		};
	private:
		explicit Module(const char *path, function_t function, Context *context);

	public:
		static Object *CREATOR(Context *_context, void *data);

		static Module *create(const char *path, function_t function, Context *context);
	};

	struct POOL_PUBLIC Parameter {
		const char *name;
		Class *type;
		bool rest;
	};

	struct POOL_PUBLIC Function : public Object {
	public:
		using function_t = Object *(*)(Object *);
		Parameter *parameters;
		long long int parameterCount;
		function_t function;
		struct FunctionData {
			Parameter *parameters;
			long long int parameterCount;
			function_t function;
		};
	private:
		Function(Parameter *parameters, long long int parameterCount, function_t function, Context *context);

	public:
		Object *call(Object *self, Object **args, size_t n) const;

		static Object *CREATOR(Context *_context, void *data);

		static Function *create(Parameter *parameters, long long int parameterCount, function_t function, Context *context);
	};

	struct POOL_PUBLIC Array : public Object {
	public:
		long long int length;
		Object **values;
		struct ArrayData {
			Object **values;
			long long int length;
		};
	private:
		explicit Array(long long int length, Object **values, Context *context);

	public:
		~Array();

		static Object *CREATOR(Context *_context, void *data);

		static Array *create(long long int length, Object **values, Context *context);
	};
}
