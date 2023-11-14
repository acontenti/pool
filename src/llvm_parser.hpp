#pragma once

#include <PoolParser.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

using namespace std;
namespace pool {
	class PoolInstanceImpl;

	class Location;

	class LLVMParser : public std::enable_shared_from_this<LLVMParser> {
	protected:
		const shared_ptr<PoolInstanceImpl> poolInstance;

		explicit LLVMParser(const shared_ptr<PoolInstanceImpl> &poolInstance);
	public:
		unique_ptr<llvm::LLVMContext> llvm_context;
		unique_ptr<llvm::Module> module;

		virtual void parseProgram(PoolParser::ProgramContext *ast) = 0;

		static shared_ptr<LLVMParser> create(const shared_ptr<PoolInstanceImpl> &poolInstance);
	};
}
