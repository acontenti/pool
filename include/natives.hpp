#pragma once

#include "poolstd.hpp"

using namespace std;

namespace llvm {
	class Value;
	class LLVMContext;
	class Module;
	class GlobalVariable;
	class Function;
}

namespace pool {
	class Natives {
	public:
		virtual bool add(const string &name, const shared_ptr<Object> &value) = 0;

		virtual bool add(const string &name, const Class::creator_t &value) = 0;

		virtual bool addFun(const string &name, const vector<Function::Param> &parameters, const NativeFunction::method_t &code) = 0;

		virtual any find(const string &name) = 0;

		POOL_PUBLIC static Natives &get();
	};

	class LLVMNatives {
	public:
		shared_ptr<llvm::LLVMContext> llvm_context;
		unique_ptr<llvm::Module> llvm_module;

		virtual llvm::Value* find(const string &name) = 0;

		virtual llvm::GlobalVariable *findConstant(const string &name) = 0;

		virtual llvm::Function* findFunction(const string &name) = 0;

		POOL_PUBLIC static shared_ptr<LLVMNatives> get();
	};
}
