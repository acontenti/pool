#pragma once

#include <sstream>
#include <chrono>

using namespace std;
using namespace chrono;

static string getExecutionTime(high_resolution_clock::time_point startTime, high_resolution_clock::time_point endTime) {
	auto execution_time_ms = duration_cast<chrono::microseconds>(endTime - startTime).count();
	auto execution_time_sec = duration_cast<chrono::seconds>(endTime - startTime).count();
	auto execution_time_min = duration_cast<chrono::minutes>(endTime - startTime).count();
	auto execution_time_hour = duration_cast<chrono::hours>(endTime - startTime).count();
	stringstream output;
	if (execution_time_hour > 0)
		output << execution_time_hour << "h, ";
	if (execution_time_min > 0)
		output << execution_time_min % 60 << "m, ";
	if (execution_time_sec > 0)
		output << execution_time_sec % 60 << "s, ";
	if (execution_time_ms > 0)
		output << execution_time_ms % long(1E+3) << "ms";
	return output.str();
}
