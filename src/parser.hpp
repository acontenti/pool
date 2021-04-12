#pragma once

#include <PoolParser.h>

using namespace std;

namespace pool {
	class Callable;

	class PoolInstanceImpl;

	[[nodiscard]] vector<shared_ptr<Callable>> parseProgram(PoolParser::ProgramContext *ast, const shared_ptr<PoolInstanceImpl> &poolInstance);
}