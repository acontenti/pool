#pragma once

#include <PoolParser.h>

namespace pool {
	class Fun;

	shared_ptr<Callable> parseCall(PoolParser::CallContext *ast, const shared_ptr<Context> &context);

	shared_ptr<Fun> parseProgram(PoolParser::ProgramContext *ast, const vector<string> &params, const shared_ptr<Context> &context);
}