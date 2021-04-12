#include "../pool_private.hpp"
#include "util/strings.hpp"
#include <regex>

#define NOMINMAX

#include "../../lib/termcolor.hpp"

#undef NOMINMAX

ostream &pool::operator<<(ostream &stream, const compile_error &error) {
	stream << termcolor::reset << endl;
	if (error.location.valid) {
		stream << error.location.module->path << ":" << error.location.start.line << ":"
			   << error.location.start.column + 1 << ": ";
	}
	stream << termcolor::red << termcolor::bold << "ERROR: " << termcolor::reset << termcolor::red << error.what()
		   << termcolor::reset << endl;
	if (error.location.valid) {
		const auto &inputStream = static_pointer_cast<PoolInstanceImpl>(error.location.module)->lexer->getInputStream();
		size_t startIndex = error.location.start.startIndex - error.location.start.column;
		misc::Interval interval(startIndex, startIndex + min<size_t>(200, inputStream->size() - startIndex));
		auto code = inputStream->getText(interval);
		code = rtrim(code.substr(0, code.find_first_of('\n')));
		code = regex_replace(code, regex("\t"), "    ");
		stream << "  " << code << endl;
		stream << "  " << termcolor::red << string(error.location.start.column, ' ') << '^';
		long long delta = error.location.end.stopIndex - error.location.start.startIndex;
		auto length = max<size_t>(0, min((size_t) llabs(delta), code.size() - error.location.start.column - 1));
		stream << string(length, '~');
		stream << termcolor::reset << endl;
	}
	return stream;
}
