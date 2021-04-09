#pragma once

#include <PoolParser.h>

namespace pool {
	class Callable;

	shared_ptr<Callable> parseExpression(PoolParser::ExpressionContext *ast, const shared_ptr<Context> &context);

	void parseProgram(PoolParser::ProgramContext *ast, const shared_ptr<Context> &context);
}