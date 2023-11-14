#include "strings.hpp"
#include <stdexcept>

namespace internal {
	static int get_codepoint(string::const_iterator &iterator) {
		// this function only makes sense after reading `\u`
		int codepoint = 0;
		const auto factors = {12u, 8u, 4u, 0u};
		for (const auto factor : factors) {
			auto current = *(++iterator);
			if (current >= '0' && current <= '9') {
				codepoint += static_cast<int>((static_cast<unsigned int>(current) - 0x30u) << factor);
			} else if (current >= 'A' && current <= 'F') {
				codepoint += static_cast<int>((static_cast<unsigned int>(current) - 0x37u) << factor);
			} else if (current >= 'a' && current <= 'f') {
				codepoint += static_cast<int>((static_cast<unsigned int>(current) - 0x57u) << factor);
			} else {
				return -1;
			}
		}
		if (codepoint < 0x0000 || codepoint > 0xFFFF)
			throw runtime_error("invalid codepoint");
		return codepoint;
	}
}

string unescapeString(const string &input) noexcept(false) {
	string result;
	auto current = input.begin();
	while (current != input.end()) {
		if (*current == '\\') {
			current++;
			if (current == input.end()) {
				throw runtime_error("invalid string: no character after backslash");
			}
			switch (*current) {
				// quotation mark
				case '\"':
					result += '\"';
					break;
					// reverse solidus
				case '\\':
					result += '\\';
					break;
					// solidus
				case '/':
					result += '/';
					break;
					// backspace
				case 'b':
					result += '\b';
					break;
					// form feed
				case 'f':
					result += '\f';
					break;
					// line feed
				case 'n':
					result += '\n';
					break;
					// carriage return
				case 'r':
					result += '\r';
					break;
					// tab
				case 't':
					result += '\t';
					break;
					// unicode escapes
				case 'u': {
					const int codepoint1 = internal::get_codepoint(current);
					int codepoint = codepoint1; // start with codepoint1

					if (codepoint1 == -1) {
						throw runtime_error("invalid string: '\\u' must be followed by 4 hex digits");
					}
					using char_int_type = string::value_type;
					// translate codepoint into bytes
					if (codepoint < 0x80) {
						// 1-byte characters: 0xxxxxxx (ASCII)
						result += static_cast<char_int_type>(codepoint);
					} else if (codepoint <= 0x7FF) {
						// 2-byte characters: 110xxxxx 10xxxxxx
						result += static_cast<char_int_type>(0xC0u | (static_cast<unsigned int>(codepoint) >> 6u));
						result += static_cast<char_int_type>(0x80u | (static_cast<unsigned int>(codepoint) & 0x3Fu));
					} else if (codepoint <= 0xFFFF) {
						// 3-byte characters: 1110xxxx 10xxxxxx 10xxxxxx
						result += static_cast<char_int_type>(0xE0u | (static_cast<unsigned int>(codepoint) >> 12u));
						result += static_cast<char_int_type>(0x80u |
															 ((static_cast<unsigned int>(codepoint) >> 6u) & 0x3Fu));
						result += static_cast<char_int_type>(0x80u | (static_cast<unsigned int>(codepoint) & 0x3Fu));
					} else {
						// 4-byte characters: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
						result += static_cast<char_int_type>(0xF0u | (static_cast<unsigned int>(codepoint) >> 18u));
						result += static_cast<char_int_type>(0x80u |
															 ((static_cast<unsigned int>(codepoint) >> 12u) & 0x3Fu));
						result += static_cast<char_int_type>(0x80u |
															 ((static_cast<unsigned int>(codepoint) >> 6u) & 0x3Fu));
						result += static_cast<char_int_type>(0x80u | (static_cast<unsigned int>(codepoint) & 0x3Fu));
					}
					break;
				}
				default:
					throw runtime_error("invalid string: forbidden character after backslash");
			}
		} else {
			result += *current;
		}
		current++;
	}
	return result;
}
