#pragma once

#include "Token.h"

using namespace antlr4;
using namespace std;

class Location {
	Location() = default;

public:
	struct Point {
		size_t line;
		size_t column;
		size_t startIndex;
		size_t stopIndex;
	} start{}, end{};
	CharStream *inputStream = nullptr;
	bool valid = false;

	Location(Token *start, Token *end);

	static Location UNKNOWN;
};
