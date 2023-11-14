#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma clang diagnostic ignored "-Wunused-value"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <poolstd.hpp>
#include "util/util_macros.hpp"
#include <iostream>

#ifndef POOLSTDLIB

#include <natives.hpp>
#include <util/errors.hpp>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>

#endif
namespace pool {
#ifndef POOLSTDLIB

	class LLVMNativesImpl : public Natives {
		using installer_fun_t = function<void(shared_ptr<LLVMNativesImpl> &)>;

		friend class Natives;

		static shared_ptr<LLVMNativesImpl> instance;
		unordered_map<string, llvm::Value *> natives;
		vector<installer_fun_t> installers;
		unique_ptr<llvm::LLVMContext> llvm_context = make_unique<llvm::LLVMContext>();
		unique_ptr<llvm::Module> llvm_module = make_unique<llvm::Module>(":std", *llvm_context);
	public:
		template<typename ...Params, typename = std::enable_if_t<(std::is_base_of_v<llvm::Type, std::remove_pointer_t<Params>> && ...)>>
		void addFunction(const string &name, llvm::Type *returnType, Params... args) {
			if (natives.find(name) != natives.end()) {
				throw compile_error("Native function '" + name + "' already exists", Location::UNKNOWN);
			}
			vector<llvm::Type *> parameters{args...};
			llvm::FunctionType *functionType = llvm::FunctionType::get(returnType, parameters, false);
			llvm::Function *function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, name, *llvm_module);
			natives[name] = function;
		}

		template<size_t... N>
		void addFunctionN(const string &name, std::index_sequence<N...>) {
			addFunction(name, llvm::PointerType::getUnqual(*llvm_context), (N, llvm::PointerType::getUnqual(*llvm_context))...);
		}

		void addConstant(const string &name) {
			if (natives.find(name) != natives.end()) {
				throw compile_error("Constant '" + name + "' already exists", Location::UNKNOWN);
			}
			auto *constant = new llvm::GlobalVariable(*llvm_module, llvm::PointerType::getUnqual(*llvm_context), true, llvm::GlobalValue::ExternalLinkage, nullptr, name);
			natives[name] = constant;
		}

		llvm::Value *find(const string &name) override {
			const auto &iterator = natives.find(name);
			if (iterator != natives.end()) {
				return iterator->second;
			} else return nullptr;
		}

		llvm::GlobalVariable *findConstant(const string &name) override {
			return reinterpret_cast<llvm::GlobalVariable *>(find(name));
		}

		llvm::Function *findFunction(const string &name) override {
			return reinterpret_cast<llvm::Function *>(find(name));
		}

		vector<llvm::Function *> getFunctions() override {
			vector<llvm::Function *> result;
			for (const auto &[_, native]: natives) {
				if (auto *function = llvm::dyn_cast<llvm::Function>(native)) {
					result.push_back(function);
				}
			}
			return result;
		}

		vector<llvm::GlobalVariable *> getConstants() override {
			vector<llvm::GlobalVariable *> result;
			for (const auto &[_, native]: natives) {
				if (auto *constant = llvm::dyn_cast<llvm::GlobalVariable>(native)) {
					result.push_back(constant);
				}
			}
			return result;
		}

		struct Installer {
			explicit Installer(const installer_fun_t &installer) {
				instance->installers.push_back(installer);
			}
		};

		void initialize(const pool::PoolVM::Settings &settings) override {
			Context::global = new Context();
			$ObjectClass = new Class("Object", nullptr, Context::global);
			$ClassClass = new Class("Class", $ObjectClass, Context::global);
			$ObjectClass->cls = $ClassClass;
			$BooleanClass = new Class("Boolean", $ObjectClass, Context::global);
			$IntegerClass = new Class("Integer", $ObjectClass, Context::global);
			$DecimalClass = new Class("Decimal", $ObjectClass, Context::global);
			$StringClass = new Class("String", $ObjectClass, Context::global);
			$ModuleClass = new Class("Module", $ObjectClass, Context::global);
			$FunctionClass = new Class("Function", $ObjectClass, Context::global);
			$ArrayClass = new Class("Array", $ObjectClass, Context::global);
			$NothingClass = new Class("Nothing", $ObjectClass, Context::global);
			$True = new Bool(true, Context::global);
			$False = new Bool(false, Context::global);
			$Null = new Nothing(Context::global);
			addConstant("$ObjectClass");
			addConstant("$ClassClass");
			addConstant("$BooleanClass");
			addConstant("$IntegerClass");
			addConstant("$DecimalClass");
			addConstant("$StringClass");
			addConstant("$ModuleClass");
			addConstant("$FunctionClass");
			addConstant("$ArrayClass");
			addConstant("$NothingClass");
			addConstant("$True");
			addConstant("$False");
			addConstant("$Null");
			addFunction("$malloc", llvm::PointerType::getUnqual(*llvm_context), llvm::Type::getInt64Ty(*llvm_context));
			for (const auto &installer: installers) {
				installer(instance);
			}
			Context::global->set("__debug__", settings.debug ? $True : $False, true);
			vector<Object *> arguments;
			arguments.reserve(settings.args.size());
			for (const auto &arg: settings.args) {
				arguments.push_back(new String(arg.c_str(), Context::global));
			}
			Context::global->set("__args__", new Array((long long int) arguments.size(), arguments.data(), Context::global), true);
		}
	};

	shared_ptr<LLVMNativesImpl> LLVMNativesImpl::instance = make_shared<LLVMNativesImpl>();

	POOL_PUBLIC shared_ptr<Natives> Natives::get() {
		return LLVMNativesImpl::instance;
	}

#endif

// NATIVE FUNCTIONS: misc
	POOL_PUBLIC EXTERN void *$malloc(long long int size) {
		return malloc(size);
	}
// NATIVE FUNCTIONS: Object
	NATIVE_FUN(String *, $Object_getContextInfo, Object *self) {
		return self->getContextInfo();
	}
	NATIVE_FUN(Class *, $Object_getClass, Object *self) {
		return self->getClass();
	}
	NATIVE_FUN(String *, $Object_getRepr, Object *self) {
		return new String(self->getDefaultRepr().c_str(), self->context);
	}
	NATIVE_FUN(Nothing *, $Object_print, Object *self) {
		String *pString = self->toString();
		std::cout << pString->value;
		delete pString;
		return $Null;
	}
	NATIVE_FUN(Nothing *, $Object_println, Object *self) {
		String *pString = self->toString();
		std::cout << pString->value << std::endl;
		delete pString;
		return $Null;
	}
	NATIVE_FUN(Bool *, $Object_instanceOf, Object *self, Class *cls) {
		return self->instanceOf(cls) ? $True : $False;
	}
	NATIVE_FUN(Object *, $Object_getLocal, Object *self, String *name) {
		auto *result = self->findLocal(name->value);
		return result ? result : $Null;
	}
	NATIVE_FUN(Object *, $Object_get, Object *self, String *name) {
		auto *result = self->find(name->value);
		return result ? result : $Null;
	}
	NATIVE_FUN(Object *, $Object_set, Object *self, String *name, Object *value, bool constant) {
		return self->context->set(name->value, value, constant);
	}
	NATIVE_FUN(Nothing *, $Object_del, Object *self, String *name) {
		self->context->del(name->value);
		return $Null;
	}
	NATIVE_FUN(Bool *, $Object_eqeq, Object *self, Object *other) {
		return (*self == other) ? $True : $False;
	}
	NATIVE_FUN(Object *, $Object_qq, Object *self, Object *other) {
		return *self == $Null ? other : self;
	}
// NATIVE FUNCTIONS: Class
	NATIVE_FUN(Class *, $Class_$new, const char *name, Class *super, Object *contextObject) {
		return new Class(name, super, contextObject->context);
	}
	NATIVE_FUN(Class *, $Class_extend, Class *self, String *name, Function *body) {
		return self->extend(name, body);
	}
	NATIVE_FUN(Object *, $Class_new, Class *self, Object **args, size_t argc) {
		return self->newInstance(self->context, args, argc);
	}
	NATIVE_FUN(Object *, $Class_getSuper, Class *self) {
		return self->super ? (Object *) self->super : $Null;
	}
	NATIVE_FUN(String *, $Class_getRepr, Class *self) {
		return self->getRepr();
	}
	NATIVE_FUN(Bool *, $Class_subclassOf, Class *self, Class *other) {
		return self->subclassOf(other) ? $True : $False;
	}
	NATIVE_FUN(Bool *, $Class_superclassOf, Class *self, Class *other) {
		return self->superclassOf(other) ? $True : $False;
	}
// NATIVE FUNCTIONS: Boolean
	NATIVE_FUN(Bool *, $Boolean_$new, bool value, Object *contextObject) {
		return new Bool(value, contextObject->context);
	}
// NATIVE FUNCTIONS: Integer
	NATIVE_FUN(Integer *, $Integer_$new, long long int value, Object *contextObject) {
		return new Integer(value, contextObject->context);
	}
// NATIVE FUNCTIONS: Decimal
	NATIVE_FUN(Decimal *, $Decimal_$new, long double value, Object *contextObject) {
		return new Decimal(value, contextObject->context);
	}
// NATIVE FUNCTIONS: String
	NATIVE_FUN(String *, $String_$new, const char *value, Object *contextObject) {
		return new String(value, contextObject->context);
	}
// NATIVE FUNCTIONS: Function
	NATIVE_FUN(Function *, $Function_$new, Parameter *parameters, long long int parameterCount, void *function, Object *contextObject) {
		return new Function(parameters, parameterCount, (Function::function_t) function, contextObject->context);
	}
	NATIVE_FUN(Object *, $Function_call, Function *self, Object *caller, Object **args, size_t argc) {
		if (self->instanceOf($FunctionClass)) {
			return self->call(caller, args, argc);
		} else {
			return $Null;
		}
	}
// NATIVE FUNCTIONS: Array
	NATIVE_FUN(Array *, $Array_$new, long long int valueCount, Object **values, Object *contextObject) {
		return new Array(valueCount, values, contextObject->context);
	}
// NATIVE FUNCTIONS: Module
	NATIVE_FUN(Module *, $Module_$new, const char *name, void *function, Object *contextObject) {
		return new Module(name, (Module::function_t) function, contextObject->context);
	}
	NATIVE_FUN(Object *, $Module_execute, Module *self) {
		return self->function(self);
	}
}
#pragma clang diagnostic pop
