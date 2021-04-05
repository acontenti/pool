#pragma once

#include <utility>

#include "location.hpp"

class compile_error : public runtime_error {
public:
	Location location;

	compile_error(const string &message, const Location &location) : runtime_error(message), location(location) {}

	inline explicit operator string() const {
		return what();
	}
};

std::ostream &operator<<(std::ostream &stream, const compile_error &error);