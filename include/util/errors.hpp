#pragma once

#include <utility>
#include <stdexcept>
#include "location.hpp"
#include "macro.hpp"

using namespace std;

class POOL_PUBLIC compile_error : public runtime_error {
public:
	Location location;

	compile_error(const string &message, const Location &location) : runtime_error(message), location(location) {}

	inline explicit operator string() const {
		return what();
	}
};

ostream &operator<<(ostream &stream, const compile_error &error);