#pragma once

#include <memory>
#include "util/location.hpp"

using namespace std;

namespace pool {
	class POOL_PUBLIC Object;

	class POOL_PUBLIC Context;

	class POOL_PUBLIC Callable {
	public:
		Location location;

		explicit Callable(const Location &location) : location(location) {}

		virtual shared_ptr<Object> invoke(const shared_ptr<Context> &context) = 0;
	};
}