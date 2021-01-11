#pragma once

#include <utility>

using namespace std;

class execution_error : public runtime_error {
public:
	execution_error(const string &file, int line, const string &message) :
			runtime_error(file + ":" + to_string(line) + ": " + message) {}
};

#define execution_error(message) execution_error(__FILE__, __LINE__, message)

class compile_error : public runtime_error {
public:
	explicit compile_error(const string &message) : runtime_error(message) {}

	inline explicit operator string() const {
		return what();
	}
};

inline ostream &operator<<(ostream &stream, const compile_error &error) noexcept {
	stream << error.what();
	return stream;
}

static constexpr inline bool endsWith(string_view str, string_view suffix) {
	return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

static constexpr inline bool startsWith(string_view str, string_view prefix) {
	return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

static inline string ltrim(string s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
	return s;
}

static inline string rtrim(string s) {
	s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
	return s;
}

static inline string trim(string s) {
	return ltrim(rtrim(move(s)));
}