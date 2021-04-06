#pragma once

#include <cstddef>
#include "macro.hpp"

using namespace std;

namespace antlr4 {
	class Token;

	class CharStream;
}

class POOL_PUBLIC Location {
	Location() = default;

public:
	struct Point {
		size_t line;
		size_t column;
		size_t startIndex;
		size_t stopIndex;
	} start{}, end{};
	antlr4::CharStream *inputStream = nullptr;
	bool valid = false;

	Location(antlr4::Token *start, antlr4::Token *end);

	static Location UNKNOWN;
};
