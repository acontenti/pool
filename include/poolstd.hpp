#pragma once

#include <util/macro.hpp>
#include <unordered_map>
#include <string>
#include <cstring>

namespace pool {
	struct Object;
	struct Class;
	struct String;
	struct Nothing;
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
	POOL_PUBLIC EXTERN Nothing *$Null;

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

	struct POOL_PUBLIC Object {
		const char *id;
		Class *cls;
		Context *context;

		Object(Class *cls, Context *context);

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
	};

	struct POOL_PUBLIC Class : public Object {
		const char *name;
		Class *super;

		explicit Class(const char *name, Class *super, Context *context)
				: Object($ClassClass, context), name(name), super(super) {}

		[[nodiscard]] Object *find(const std::string &id) const;

		[[nodiscard]] Object *findInClass(const std::string &id) const;

		Object *newInstance(Context *parent, Object **args, size_t argc) const;

		Class *extend(String *className, Function *block) const;

		bool subclassOf(Class *_cls) const;

		bool superclassOf(Class *_cls) const;

		[[nodiscard]] String *getRepr();

		bool operator==(const Class *rhs) const;
	};

	struct POOL_PUBLIC Bool : public Object {
		bool value;

		explicit Bool(bool value, Context *context) : Object($BooleanClass, context), value(value) {}
	};

	struct POOL_PUBLIC Integer : public Object {
		long long int value;

		explicit Integer(long long int value, Context *context) : Object($IntegerClass, context), value(value) {}
	};

	struct POOL_PUBLIC Decimal : public Object {
		long double value;

		explicit Decimal(long double value, Context *context) : Object($DecimalClass, context), value(value) {}
	};

	struct POOL_PUBLIC String : public Object {
		const char *value;
		size_t length;

		explicit String(const char *value, Context *context) : Object($StringClass, context) {
			this->value = strdup(value);
			this->length = strlen(value);
		}
	};

	struct POOL_PUBLIC Module : public Object {
		typedef Object *(*function_t)(Object *);

		const char *path;
		function_t function;

		explicit Module(const char *path, function_t function, Context *context)
				: Object($ModuleClass, context), path(path), function(function) {}
	};

	struct POOL_PUBLIC Parameter {
		const char *name;
		Class *type;
		bool rest;
	};

	struct POOL_PUBLIC Function : public Object {
		typedef Object *(*function_t)(Object *);

		Parameter *parameters;
		long long int parameterCount;
		function_t function;

		Function(Parameter *parameters, long long int parameterCount, function_t function, Context *context)
				: Object($FunctionClass, context), parameters(parameters), parameterCount(parameterCount),
				  function(function) {}

		Object *call(Object *self, Object **args, size_t n) const;
	};

	struct POOL_PUBLIC Array : public Object {
		long long int length;
		Object **values;

		explicit Array(long long int length, Object **values, Context *context);

		~Array();
	};

	struct POOL_PUBLIC Nothing : public Object {
		explicit Nothing(Context *context) : Object($NothingClass, context) {}
	};
}
