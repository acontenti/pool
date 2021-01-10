#pragma once

#include <vector>
#include <memory>
#include "../gen/PoolParserBaseListener.h"
#include "../gen/PoolLexer.h"

using namespace std;
using namespace antlr4;

namespace pool {
	struct Settings {
		bool debug;
		const vector<string> &args;
	};

	class compile_error : public std::runtime_error {
	public:
		explicit compile_error(const std::string &message) : std::runtime_error(message) {}

		inline const string message() const noexcept {
			return runtime_error::what();
		}
	};

	class PoolX : PoolParserBaseListener {
		explicit PoolX(const string &file);

	public:
		constexpr static const string_view VERSION = POOL_VERSION;
		constexpr static const string_view EXT = ".pool";

		static void setOptions(const Settings &settings);

		static PoolX execute(const string &filename);

		static void compile_warning(const std::string &message, antlr4::Token *token = nullptr) noexcept;

		static void compile_error(const std::string &message, antlr4::Token *token = nullptr) noexcept;

		static pool::compile_error compile_fatal(const std::string &message, antlr4::Token *token = nullptr) noexcept;
	};
}