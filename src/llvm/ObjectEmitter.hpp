#pragma once

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/MC/MCTargetOptions.h>
#include <llvm/Support/TargetSelect.h>

namespace pool {
	class ObjectEmitter {
	public:
		static void initialize() {
			llvm::InitializeNativeTarget();
			llvm::InitializeNativeTargetAsmPrinter();
		}

		static std::string emit(const std::unique_ptr<llvm::Module> &module, const std::string &filename) {
			std::string error;
			auto targetTriple = llvm::sys::getDefaultTargetTriple();
			auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
			if (!target) {
				return error;
			}
			auto cpu = "generic";
			auto features = "";
			llvm::TargetOptions opt;
			auto RM = std::optional<llvm::Reloc::Model>(llvm::Reloc::Model::PIC_);
			auto targetMachine = target->createTargetMachine(targetTriple, cpu, features, opt, RM, nullopt, llvm::CodeGenOpt::Default);
			auto dataLayout = targetMachine->createDataLayout();
			module->setDataLayout(dataLayout);
			module->setTargetTriple(targetTriple);
			module->dump();
			auto &stdLib = LLVMNatives::get()->llvm_module;
			// copy everything from stdLib to module
			for (auto &global: stdLib->globals()) {
				module->getOrInsertGlobal(global.getName(), global.getValueType());
			}
			for (auto &function: stdLib->functions()) {
				module->getOrInsertFunction(function.getName(), function.getFunctionType());
			}
			std::error_code ec;
			llvm::raw_fd_ostream dest(filename, ec, llvm::sys::fs::OF_None);
			if (ec) {
				return "Could not open file: " + ec.message();
			}
			llvm::legacy::PassManager pass;
			auto FileType = llvm::CGFT_ObjectFile;
			if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
				return "TargetMachine can't emit a file of this type";
			}
			pass.run(*module);
			dest.flush();
			return "";
		}
	};
}
