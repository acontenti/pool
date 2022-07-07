#pragma once

#include <memory>
#include "util/location.hpp"

using namespace std;

namespace pool {
	class Object;

	class Context;

	class Callable {
	public:
		Location location;

		explicit Callable(Location location) : location(move(location)) {}

		virtual shared_ptr<Object> invoke(const shared_ptr<Context> &context) = 0;
	};
}