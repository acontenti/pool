#pragma once

#include <cstddef>
#include <memory>
#include "macro.hpp"

using namespace std;

namespace pool {
	class POOL_PUBLIC PoolInstance;

	class POOL_PUBLIC Location {
		Location() = default;

	public:
		struct Point {
			size_t line;
			size_t column;
			size_t startIndex;
			size_t stopIndex;
		} start{}, end{};
		shared_ptr<PoolInstance> module;
		bool valid = false;

		Location(const Point &start, const Point &end, const shared_ptr<PoolInstance>& module);

		static Location UNKNOWN;
	};
}