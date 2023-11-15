#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <poolstd.hpp>
#include "util/strings.hpp"
#include <random>
#include <sstream>

namespace pool {
	template<typename T>
	constexpr inline const T reconstruct(void *data) {
		T _data = *static_cast<T *>(data);
		delete static_cast<T *>(data);
		return _data;
	}

	string generateObjectID() {
		static std::random_device rd;
		static std::mt19937 mt(rd());
		static std::uniform_int_distribution<int64_t> dist(0, 0x7FFFFFFFFFFFFFFF);
		int64_t random = dist(mt);
		return to_string_base<62>(random);
	}

	Context *Context::global = nullptr;
	Class *$ObjectClass = nullptr;
	Class *$ClassClass = nullptr;
	Class *$BooleanClass = nullptr;
	Class *$IntegerClass = nullptr;
	Class *$DecimalClass = nullptr;
	Class *$StringClass = nullptr;
	Class *$ModuleClass = nullptr;
	Class *$FunctionClass = nullptr;
	Class *$ArrayClass = nullptr;
	Class *$NothingClass = nullptr;
	Bool *$True = nullptr;
	Bool *$False = nullptr;
	Object *$Null = nullptr;

	std::string Context::toString() const {
		stringstream ss;
		ss << "{ ";
		if (!heap.empty()) {
			for (const auto &[name, value]: heap) {
				ss << name << ":" << value.first->getRepr() << ", ";
			}
		}
		ss << "}";
		return ss.str();
	}

	Object *Context::findLocal(const string &name) const {
		if (auto it = heap.find(name); it != heap.end()) {
			return it->second.first;
		} else return nullptr;
	}

	Object *Context::get(const std::string &name) const {
		if (auto local = findLocal(name)) {
			return local;
		}
		if (parent) {
			return parent->get(name);
		}
		return nullptr;
	}

	Object *Context::set(const std::string &name, Object *value, bool constant) {
		const auto &it = heap.find(name);
		if (it != heap.end()) {
			if (it->second.second) {
				throw runtime_error("Cannot reassign constant '" + name + "'");
			}
			it->second.first = value;
		} else {
			heap[name] = {value, constant};
		}
		return value;
	}

	void Context::del(const string &name) {
		heap.erase(name);
	}

	Object::Object(Class *cls, Context *context) : cls(cls), context(new Context(context)) {
		this->id = strdup(generateObjectID().c_str());
	}

	Class *Object::getClass() const {
		return cls;
	}

	std::string Object::getDefaultRepr() const {
		return string(cls->name) + "@" + id;
	}

	String *Object::getRepr() {
		if (auto method = findMethod("getRepr")) {
			if (Object *result = method->call(this, {}, 0)) {
				if (result->instanceOf($StringClass)) {
					return (String *) result;
				}
			}
		}
		return String::create(getDefaultRepr().c_str(), context);
	}

	String *Object::getContextInfo() const {
		return String::create(context->toString().c_str(), context);
	}

	Function *Object::findMethod(const string &name) const {
		if (Object *ptr = cls->findInClass(name)) {
			if (ptr->getClass() == $FunctionClass) {
				return (Function *) ptr;
			} else return nullptr;
		} else return nullptr;
	}

	Object *Object::find(const string &name) const {
		if (auto *local = context->findLocal(name)) {
			return local;
		}
		return getClass()->findInClass(name);
	}

	Object *Object::findLocal(const string &name) const {
		return context->findLocal(name);
	}

	Bool *Object::instanceOf(Class *_cls) const {
		if (*this->cls == _cls || this->cls->subclassOf(_cls)) {
			return $True;
		} else return $False;
	}

	String *Object::toString() {
		if (auto *method = findMethod("toString")) {
			if (Object *result = method->call(this, {}, 0)) {
				if (result->instanceOf($StringClass)) {
					return (String *) result;
				}
			}
		}
		return getRepr();
	}

	bool Object::operator==(const Object *rhs) const {
		return strcmp(id, rhs->id) == 0;
	}

	Object *Object::CREATOR(Context *_context, void *data) {
		return new Object(static_cast<Class *>(data), _context);
	};

	Object *Class::find(const string &id) const {
		if (auto local = findInClass(id)) {
			return local;
		}
		if (cls) { // if not $ClassClass
			return cls->find(id);
		}
		return nullptr;
	}

	Object *Class::findInClass(const string &id) const {
		if (auto local = context->findLocal(id)) {
			return local;
		}
		if (super) {
			return super->findInClass(id);
		}
		return nullptr;
	}

	Object *Class::newInstance(Context *parent, void *data, Object **args, size_t argc) const {
		auto *instance = creator(parent, data);
		instance->cls = const_cast<Class *>(this);
		if (auto method = instance->findMethod("init")) {
			method->call(instance, args, argc);
		}
		return instance;
	}

	Class *Class::extend(const char *className, creator_t _creator, Function *block) const {
		auto *newClass = Class::create(className, this, _creator, context);
		if (block) {
			block->context->set("class", (Object *) newClass, true);
			block->context->set("super", (Object *) this, true);
			block->call(newClass, {}, 0);
			for (const auto &[_name, value]: *block->context) {
				newClass->context->set(_name, value.first, value.second);
			}
		}
		return newClass;
	}

	Class *Class::extend(Class *newClass, Function *block) const {
		if (block) {
			block->context->set("class", (Object *) newClass, true);
			block->context->set("super", (Object *) this, true);
			block->call(newClass, {}, 0);
			for (const auto &[_name, value]: *block->context) {
				newClass->context->set(_name, value.first, value.second);
			}
		}
		return static_cast<Class *>(newClass);
	}

	bool Class::subclassOf(const Class *_cls) const {
		return super && (*super == _cls || super->subclassOf(_cls));
	}

	bool Class::superclassOf(const Class *_cls) const {
		return _cls->super && (*this == _cls->super || superclassOf(_cls->super));
	}

	bool Class::operator==(const Class *rhs) const {
		return std::strcmp(name, rhs->name) == 0 && std::strcmp(id, rhs->id) == 0;
	}

	String *Class::getRepr() {
		return String::create((getDefaultRepr() + "[" + name + "]").c_str(), context);
	}

	Object *Class::CREATOR(Context *_context, void *data) {
		const auto &classData = reconstruct<ClassData>(data);
		return new Class(classData.name, classData.super, classData.creator, _context);
	};

	Class *Class::create(const char *className, const Class *super, const creator_t &creator, Context *context) {
		return static_cast<Class *>($ClassClass->newInstance(context, new ClassData{className, super, creator}));
	}

	Bool::Bool(bool value, Context *context) : Object($BooleanClass, context), value(value) {}

	Object *Bool::CREATOR(Context *_context, void *data) {
		return new Bool(reconstruct<bool>(data), _context);
	};

	Integer::Integer(long long int value, Context *context) : Object($IntegerClass, context), value(value) {}

	Integer *Integer::create(long long int value, Context *context) {
		return static_cast<Integer *>($IntegerClass->newInstance(context, new long long int(value)));
	}

	Object *Integer::CREATOR(Context *_context, void *data) {
		return new Integer(reconstruct<long long int>(data), _context);
	};

	Decimal::Decimal(long double value, Context *context) : Object($DecimalClass, context), value(value) {}

	Decimal *Decimal::create(long double value, Context *context) {
		return static_cast<Decimal *>($DecimalClass->newInstance(context, new long double(value)));
	}

	Object *Decimal::CREATOR(Context *_context, void *data) {
		return new Decimal(reconstruct<long double>(data), _context);
	};

	String::String(const char *value, Context *context) : Object($StringClass, context) {
		this->value = strdup(value);
		this->length = strlen(value);
	}

	String *String::create(const char *value, Context *context) {
		return static_cast<String *>($StringClass->newInstance(context, (void *) value));
	}

	Object *String::CREATOR(Context *_context, void *data) {
		return new String(static_cast<const char *>(data), _context);
	}

	Module::Module(const char *path, Module::function_t function, Context *context)
			: Object($ModuleClass, context), path(path), function(function) {}

	Module *Module::create(const char *path, Module::function_t function, Context *context) {
		return static_cast<Module *>($ModuleClass->newInstance(context, new Module::ModuleData{path, function}));
	}

	Object *Module::CREATOR(Context *_context, void *data) {
		const auto &moduleData = reconstruct<ModuleData>(data);
		return new Module(moduleData.path, moduleData.function, _context);
	};

	Function::Function(Parameter *parameters, long long int parameterCount, Function::function_t function, Context *context)
			: Object($FunctionClass, context), parameters(parameters), parameterCount(parameterCount),
			  function(function) {}

	Object *Function::call(Object *self, Object **other, size_t n) const {
		vector<Object *> args;
		if (*this == self) {
			args.reserve(n);
		} else {
			args.reserve(n + 1);
			args.push_back(self);
		}
		if (other && n) {
			args.insert(args.end(), other, other + n);
		}
		Parameter *lastParameter = parameters + parameterCount - 1;
		const auto hasRest = parameters != nullptr && lastParameter->rest;
		if (hasRest) {
			if (args.size() < parameterCount - 1)
				throw runtime_error("Function takes at least " + to_string(parameterCount - 1)
									+ " arguments, but " + to_string(args.size()) + " were given");
		} else if (parameterCount != args.size()) {
			throw runtime_error("Function takes " + to_string(parameterCount)
								+ " arguments, but " + to_string(args.size()) + " were given");
		}
		for (int i = 0; i < parameterCount; ++i) {
			const auto &param = parameters[i];
			if (!param.rest) {
				const auto &value = args[i];
				if (param.type) {
					if (!value->instanceOf(param.type)) {
						throw runtime_error("Argument for function parameter '" + string(param.name) + "' must be of class '" + param.type->name
											+ "', but value of class '" + value->getClass()->name + "' was given");
					}
				}
				context->set(param.name, value, true);
			} else {
				std::vector<Object *> rest{args.begin() + i, args.end()};
				context->set(param.name, Array::create((long long int) rest.size(), rest.data(), context), true);
			}
		}
		return function((Object *) this);
	}

	Function *Function::create(Parameter *parameters, long long int parameterCount, Function::function_t function, Context *context) {
		return static_cast<Function *>($FunctionClass->newInstance(context, new Function::FunctionData{parameters, parameterCount, function}));
	}

	Object *Function::CREATOR(Context *_context, void *data) {
		const auto &functionData = reconstruct<FunctionData>(data);
		return new Function(functionData.parameters, functionData.parameterCount, functionData.function, _context);
	};

	Array::Array(long long int length, Object **values, Context *context)
			: Object($ArrayClass, context), length(length) {
		this->values = new Object *[length];
		for (int i = 0; i < length; ++i) {
			this->values[i] = values[i];
		}
	}

	Array::~Array() {
		delete[] values;
	}

	Array *Array::create(long long int length, Object **values, Context *context) {
		return static_cast<Array *>($ArrayClass->newInstance(context, new Array::ArrayData{values, length}));
	}

	Object *Array::CREATOR(Context *_context, void *data) {
		const auto &arrayData = reconstruct<ArrayData>(data);
		return new Array(arrayData.length, arrayData.values, _context);
	};
}
#pragma clang diagnostic pop
