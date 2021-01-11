#pragma once

#include <utility>

class execution_error : public std::runtime_error {
public:
	execution_error(const std::string &file, int line, const std::string &message) :
			runtime_error(file + ":" + std::to_string(line) + ": " + message) {}
};

#define execution_error(message) execution_error(__FILE__, __LINE__, message)

class compile_error : public std::runtime_error {
public:
	explicit compile_error(const std::string &message) : runtime_error(message) {}

	inline explicit operator std::string() const {
		return what();
	}
};

inline std::ostream &operator<<(std::ostream &stream, const compile_error &error) noexcept {
	stream << error.what();
	return stream;
}

static constexpr inline bool endsWith(std::string_view str, std::string_view suffix) {
	return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

static constexpr inline bool startsWith(std::string_view str, std::string_view prefix) {
	return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

static inline std::string ltrim(std::string s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c);}));
	return s;
}

static inline std::string rtrim(std::string s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int c) {return !std::isspace(c);}).base(), s.end());
	return s;
}

static inline std::string trim(std::string s) {
	return ltrim(rtrim(move(s)));
}