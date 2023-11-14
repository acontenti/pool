#pragma once

#include <pool.hpp>
#include <util/errors.hpp>
#include <PoolLexer.h>
#include <PoolParser.h>

using namespace antlr4;

namespace llvm {
	class LLVMContext;
	class Module;
}

namespace pool {
	class PoolInstanceImpl : public PoolInstance,
							 public BaseErrorListener,
							 public enable_shared_from_this<PoolInstanceImpl> {
	public:
		unique_ptr<ANTLRInputStream> inputStream;
		unique_ptr<PoolLexer> lexer;
		unique_ptr<CommonTokenStream> tokens;
		unique_ptr<PoolParser> parser;

		explicit PoolInstanceImpl(const string &path);

		void syntaxError(antlr4::Recognizer *, antlr4::Token *token, size_t, size_t, const string &msg, exception_ptr) override {
			Location::Point point{token->getLine(), token->getCharPositionInLine(), token->getStartIndex(), token->getStopIndex()};
			cerr << compile_error(msg, Location{point, point, shared_from_this()});
		}

		shared_ptr<pool::Module> execute() noexcept(false) override;

		void compile() noexcept(false) override;
	};

	class PoolVMImpl : public PoolVM {
	public:
		shared_ptr<llvm::LLVMContext> llvm_context;
		static shared_ptr<PoolVMImpl> instance;

		shared_ptr<Module> execute(const string &moduleName) noexcept(false) override;

		void compile(const string &module) noexcept(false) override;

		static inline shared_ptr<PoolVMImpl> get() {
			return instance;
		}
	};

	void initializeStdLib();
}
