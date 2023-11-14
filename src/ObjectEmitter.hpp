#pragma once

#include <memory>
#include <string>

namespace llvm {
	class Module;
}
namespace pool {
	class ObjectEmitter {
	public:
		static void initialize();

		static std::string emit(const std::unique_ptr<llvm::Module> &module, const std::string &filename);
	};
}
