#pragma once

class execution_error : public std::runtime_error {
public:
	execution_error(const std::string &file, int line, const std::string &message) : runtime_error(
			file + ":" + std::to_string(line) + ": " + message) {}
};
