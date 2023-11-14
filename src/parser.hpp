#pragma once

#include <PoolParser.h>

using namespace std;
namespace llvm {
	class Module;

	class LLVMContext;
}
namespace pool {
	class PoolInstanceImpl;

	class Location;

	class Parser : public std::enable_shared_from_this<Parser> {
	protected:
		const shared_ptr<PoolInstanceImpl> poolInstance;

		explicit Parser(const shared_ptr<PoolInstanceImpl> &poolInstance);

	public:
		unique_ptr<llvm::LLVMContext> llvm_context;
		unique_ptr<llvm::Module> module;

		virtual void parseProgram(PoolParser::ProgramContext *ast) = 0;

		static shared_ptr<Parser> create(const shared_ptr<PoolInstanceImpl> &poolInstance);
	};
}
