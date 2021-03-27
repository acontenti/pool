#include <iostream>
#include "pool.hpp"
#include "util/time.hpp"
#include "../lib/argagg.hpp"
#include "../lib/termcolor.hpp"

using namespace pool;

int main(int argc, char **argv) {
	bool debug;
	auto startTime = high_resolution_clock::now();
	try {
		vector<argagg::definition> options{
				{"debug", vector<string>{"-d", "--debug"}, "enables debug options", 0}
		};
		argagg::parser argparser{options};
		argagg::parser_results result = argparser.parse(argc, argv);
		debug = result["debug"];
		if (debug || result.pos.empty()) {
			cout << "pool " << pool::Pool::VERSION << endl;
		}
		if (!result.pos.empty()) {
			vector<string> args;
			string filename = result.pos[0];
			for (int i = 1; i < result.pos.size(); ++i) {
				args.emplace_back(result.pos[i]);
			}
			Pool::initialiaze({debug, args});
			auto pool = Pool::execute(filename);
			if (debug) {
				auto executionTime = getExecutionTime(startTime, high_resolution_clock::now());
				if (pool.getResult()) {
					std::cout << std::endl << termcolor::green << "Execution successful" << termcolor::reset;
				} else {
					std::cout << std::endl << termcolor::red << "Execution failed" << termcolor::reset;
				}
				std::cout << " in " << executionTime << std::endl << std::endl;
			}
		}
		return EXIT_SUCCESS;
	} catch (const exception &e) {
		cerr << e.what() << endl;
		if (debug) {
			cout << termcolor::red << "Execution failed" << termcolor::reset
				 << " in " << getExecutionTime(startTime, high_resolution_clock::now())
				 << endl << endl;
		}
		return EXIT_FAILURE;
	}
}

