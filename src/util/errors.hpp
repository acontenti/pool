#pragma once

#include "Token.h"

using namespace std;
using namespace antlr4;

#define execution_error(message) runtime_error(__FILE__ ":" + to_string(__LINE__) + ": " + (message))

class compile_error : public runtime_error {
public:
	antlr4::Token *const start;
	antlr4::Token *const end;

	compile_error(const string &message, antlr4::Token *start, antlr4::Token *end)
			: runtime_error(message), start(start), end(end) {}

	inline explicit operator string() const {
		return what();
	}

};

std::ostream &operator<<(std::ostream &stream, const compile_error &error);