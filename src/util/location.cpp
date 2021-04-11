#include "util/location.hpp"
#include <Token.h>
#include <CharStream.h>

Location Location::UNKNOWN{};

Location::Location(antlr4::Token *start, antlr4::Token *end) {
	if (start && end) {
		this->start = Point{start->getLine(), start->getCharPositionInLine(), start->getStartIndex(), start->getStopIndex()};
		this->end = Point{end->getLine(), end->getCharPositionInLine(), end->getStartIndex(), end->getStopIndex()};
		this->inputStream = start->getInputStream();
		this->valid = true;
	}
}