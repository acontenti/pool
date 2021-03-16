#pragma once

#include <vector>
#include <memory>
#include "Token.h"
#include "util.hpp"

using namespace std;
using namespace antlr4;

namespace pool {
	struct Settings {
		bool debug;
		const vector<string> &args;
	};

	class Pool {
		Pool(const string &filename, istream &stream);

	public:
		constexpr static const string_view VERSION = POOL_VERSION;
		constexpr static const string_view EXT = ".pool";

		static void initialiaze(const Settings &settings);

		static Pool execute(const string &filename);

		static void compile_error(const string &message, Token *token, ostream &stream) noexcept;

		static void compile_error(const string &message, Token *token, size_t line, size_t col, ostream &stream) noexcept;

		static ::compile_error compile_fatal(const string &message, Token *token = nullptr) noexcept;
	};
}