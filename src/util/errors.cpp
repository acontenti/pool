#include "errors.hpp"
#include "strings.hpp"
#include <cmath>
#include <CharStream.h>

#define NOMINMAX

#include "../../lib/termcolor.hpp"

#undef NOMINMAX

std::ostream &operator<<(ostream &stream, const compile_error &error) {
	stream << termcolor::reset << endl;
	if (error.location.valid) {
		stream << error.location.inputStream->getSourceName() << ":" << error.location.start.line << ":"
			   << error.location.start.column + 1 << ": ";
	}
	stream << termcolor::red << termcolor::bold << "ERROR: " << termcolor::reset << termcolor::red << error.what()
		   << termcolor::reset << endl;
	if (error.location.valid) {
		size_t startIndex = error.location.start.startIndex - error.location.start.column;
		misc::Interval interval(startIndex, startIndex + min<size_t>(200, error.location.inputStream->size() - startIndex));
		auto code = error.location.inputStream->getText(interval);
		code = rtrim(code.substr(0, code.find_first_of('\n')));
		stream << "  " << code << endl;
		stream << "  " << termcolor::red << string(error.location.start.column, ' ') << '^';
		make_signed<size_t>::type delta = error.location.end.stopIndex - error.location.start.startIndex;
		auto length = max<size_t>(0, min((size_t) llabs(delta), code.size() - error.location.start.column - 1));
		stream << string(length, '~');
		stream << termcolor::reset << endl;
	}
	return stream;
}
