#pragma once

#include <iostream>
#include <string>
#include <BaseErrorListener.h>

using namespace std;
using namespace antlr4;

class ErrorListener : public BaseErrorListener {
	void syntaxError(Recognizer *recognizer, Token *offendingSymbol, size_t line, size_t charPositionInLine, const string &msg, exception_ptr e) override {
		cerr << compile_error(msg, offendingSymbol, offendingSymbol);
	}

public:
	static ErrorListener *INSTANCE() {
		static ErrorListener instance;
		return &instance;
	};
};
