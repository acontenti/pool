#pragma once

#include <PoolParser.h>

namespace pool {
	class Fun;

	shared_ptr<Callable> parseCall(PoolParser::CallContext *ast, const shared_ptr<Context> &context);

	void parseProgram(PoolParser::ProgramContext *ast, const shared_ptr<Context> &context);
}