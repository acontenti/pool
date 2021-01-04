#pragma once

#include <experimental/source_location>

using source_location = std::experimental::source_location;

class execution_error : public std::runtime_error {
public:
	explicit execution_error(const std::string &message, const source_location &location = source_location::current())
			: runtime_error(std::string(location.file_name()) + ":" + std::to_string(location.line()) + ":" +
							std::to_string(location.column()) + ": " + message) {}
};

static bool endsWith(std::string_view str, std::string_view suffix) {
	return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

static bool startsWith(std::string_view str, std::string_view prefix) {
	return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}