#pragma once

#include "Token.h"

using namespace std;
using namespace antlr4;

class compile_error : public runtime_error {
public:
	antlr4::Token *const start;
	antlr4::Token *const end;

	compile_error(const string &message, antlr4::Token *start, antlr4::Token *end)
			: runtime_error(message), start(start), end(end) {}

	compile_error(const string &message, const pair<Token *, Token *> &location)
			: runtime_error(message), start(location.first), end(location.second) {}

	inline explicit operator string() const {
		return what();
	}

};

std::ostream &operator<<(std::ostream &stream, const compile_error &error);