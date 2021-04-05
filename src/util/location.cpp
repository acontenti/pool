#include "location.hpp"
#include "CharStream.h"

Location Location::UNKNOWN{};

Location::Location(Token *start, Token *end) {
	if (start && end) {
		this->start = Point{start->getLine(), start->getCharPositionInLine(), start->getStartIndex(), start->getStopIndex()};
		this->end = Point{end->getLine(), end->getCharPositionInLine(), end->getStartIndex(), end->getStopIndex()};
		this->inputStream = start->getInputStream();
		this->valid = true;
	}
}