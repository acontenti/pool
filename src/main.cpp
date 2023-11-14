#include <pool.hpp>
#include "util/time.hpp"
#include <util/errors.hpp>
#include "../lib/argagg.hpp"
#include "../lib/termcolor.hpp"
#include <iostream>
#include <cpplocate/cpplocate.h>

using namespace pool;

int main(int argc, char **argv) {
	bool debug;
	auto startTime = high_resolution_clock::now();
	try {
		vector<argagg::definition> options{
				{"debug", vector<string>{"-d", "--debug"}, "enables debug options", 0},
				{"help", vector<string>{"-h", "--help"}, "shows this help", 0},
				{"locate", vector<string>{"-l", "--locate"}, "shows executable location", 0},
		};
		argagg::parser argparser{options};
		argagg::parser_results arguments = argparser.parse(argc, argv);
		bool showHelp = arguments["help"] || arguments.pos.empty();
		if (!arguments["locate"]) {
			cerr << "pool " << PoolVM::getVersion() << endl;
		}
		if (arguments["locate"]) {
			cerr << PoolVM::getSDKPath() << endl;
			return EXIT_SUCCESS;
		} else if (showHelp) {
			cerr << "Usage: pool [options] file" << endl << argparser;
			return EXIT_SUCCESS;
		} else {
			vector<string> args;
			string filename = arguments.pos[0];
			for (int i = 1; i < arguments.pos.size(); ++i) {
				args.emplace_back(arguments.pos[i]);
			}
			PoolVM::initialiaze({arguments["debug"], args});
			PoolVM::get()->compile(filename);
			auto executionTime = getExecutionTime(startTime, high_resolution_clock::now());
			cerr << endl << termcolor::bright_green << "Compilation successful" << termcolor::reset;
			cerr << " in " << executionTime << endl << endl;
			return EXIT_SUCCESS;
		}
	} catch (const compile_error &e) {
		cerr << e << endl;
		auto executionTime = getExecutionTime(startTime, high_resolution_clock::now());
		cerr << endl << termcolor::red << "Compilation failed" << termcolor::reset;
		cerr << " in " << executionTime << endl << endl;
		return EXIT_FAILURE;
	} catch (const exception &e) {
		cerr << termcolor::red << e.what() << termcolor::reset << endl;
		if (debug) {
			auto executionTime = getExecutionTime(startTime, high_resolution_clock::now());
			cerr << endl << termcolor::red << "Execution failed" << termcolor::reset;
			cerr << " in " << executionTime << endl << endl;
		}
		return EXIT_FAILURE;
	}
}

