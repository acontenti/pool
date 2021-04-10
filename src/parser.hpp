#pragma once

#include <PoolParser.h>

using namespace std;

namespace pool {
	class Callable;

	[[nodiscard]] vector<shared_ptr<Callable>> parseProgram(PoolParser::ProgramContext *ast);
}