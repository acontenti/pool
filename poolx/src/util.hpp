#pragma once

class execution_error : public std::runtime_error {
public:
	execution_error(const std::string &file, int line, const std::string &message) :
			runtime_error(file + ":" + std::to_string(line) + ": " + message) {}
};

#define execution_error(message) execution_error(__FILE__, __LINE__, message)

static bool endsWith(std::string_view str, std::string_view suffix) {
	return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

static bool startsWith(std::string_view str, std::string_view prefix) {
	return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}