#include "util/location.hpp"

using namespace pool;

Location Location::UNKNOWN{};

Location::Location(const Point &start, const Point &end, const shared_ptr<PoolInstance> &module)
		: start(start), end(end), module(module), valid(true) {}
