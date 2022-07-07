#pragma once

#include <utility>
#include <stdexcept>
#include "location.hpp"
#include "macro.hpp"

using namespace std;

namespace pool {
	class compile_error : public runtime_error {
	public:
		Location location;

		compile_error(const string &message, Location location) : runtime_error(message), location(move(location)) {}

		inline explicit operator string() const {
			return what();
		}
	};

	POOL_PUBLIC ostream &operator<<(ostream &stream, const compile_error &error);
}