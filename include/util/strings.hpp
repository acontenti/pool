#pragma once

#include <string>
#include <algorithm>
#include "macro.hpp"

using namespace std;

POOL_PUBLIC string unescapeString(const string &input) noexcept(false);

static constexpr inline bool endsWith(string_view str, string_view suffix) {
	return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

static constexpr inline bool startsWith(string_view str, string_view prefix) {
	return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

static inline string ltrim(string s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), [](int c) { return !isspace(c); }));
	return s;
}

static inline string rtrim(string s) {
	s.erase(find_if(s.rbegin(), s.rend(), [](int c) { return !isspace(c); }).base(), s.end());
	return s;
}

static inline string trim(string s) {
	return ltrim(rtrim(move(s)));
}