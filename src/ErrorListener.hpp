#pragma once

#include <string>
#include <BaseErrorListener.h>

using namespace antlr4;

class ErrorListener : public BaseErrorListener {
	void syntaxError(Recognizer *recognizer, Token *offendingSymbol, size_t line, size_t charPositionInLine, const std::string &msg, std::exception_ptr e) override {
		pool::Pool::compile_error(msg, offendingSymbol, line, charPositionInLine, cout);
	}

public:
	static ErrorListener *INSTANCE() {
		static ErrorListener instance;
		return &instance;
	};
};
