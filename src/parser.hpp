#pragma once

#include <PoolParser.h>

using namespace std;

namespace pool {
	class Callable;

	class PoolInstanceImpl;

	class Location;

	class Parser : public enable_shared_from_this<Parser> {
	protected:
		const shared_ptr<PoolInstanceImpl> poolInstance;

		explicit Parser(const shared_ptr<PoolInstanceImpl> &poolInstance) : poolInstance(poolInstance) {};

	public:
		virtual vector<shared_ptr<Callable>> parseProgram(PoolParser::ProgramContext *ast) = 0;

		static shared_ptr<Parser> create(const shared_ptr<PoolInstanceImpl> &poolInstance);
	};
}
