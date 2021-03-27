#include "errors.hpp"
#include "strings.hpp"
#include "CharStream.h"
#include "../../lib/termcolor.hpp"

std::ostream &operator<<(ostream &stream, const compile_error &error) {
	stream << termcolor::reset << endl;
	if (error.start && error.end) {
		stream << error.start->getInputStream()->getSourceName() << ":" << error.start->getLine() << ":"
			   << error.start->getCharPositionInLine() + 1 << ": ";
	}
	stream << termcolor::red << termcolor::bold << "ERROR: " << termcolor::reset << termcolor::red << error.what()
		   << termcolor::reset << endl;
	if (error.start && error.end) {
		CharStream *inputStream = error.start->getInputStream();
		size_t startIndex = error.start->getStartIndex() - error.start->getCharPositionInLine();
		misc::Interval interval(startIndex, startIndex + min(200ul, inputStream->size() - startIndex));
		auto code = inputStream->getText(interval);
		code = rtrim(code.substr(0, code.find_first_of('\n')));
		stream << "  " << code << endl;
		stream << "  " << termcolor::red << string(error.start->getCharPositionInLine(), ' ') << '^';
		make_signed<size_t>::type delta = error.end->getStopIndex() - error.start->getStartIndex();
		stream << string(llabs(delta), '~');
		stream << termcolor::reset << endl;
	}
	return stream;
}
