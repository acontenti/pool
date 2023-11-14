#pragma once

#include "pool.hpp"
#include <memory>
#include <vector>
#include <string>

using namespace std;

namespace llvm {
	class Value;
	class LLVMContext;
	class GlobalVariable;
	class Function;
}

namespace pool {
	class LLVMNatives {
	public:
		virtual llvm::Value* find(const string &name) = 0;

		virtual llvm::GlobalVariable *findConstant(const string &name) = 0;

		virtual llvm::Function* findFunction(const string &name) = 0;

		virtual vector<llvm::Function*> getFunctions() = 0;

		virtual vector<llvm::GlobalVariable*> getConstants() = 0;

		virtual void initialize(const pool::PoolVM::Settings settings) = 0;

		POOL_PUBLIC static shared_ptr<LLVMNatives> get();
	};
}
