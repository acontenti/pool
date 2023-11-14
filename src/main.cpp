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
				{"compile", vector<string>{"-c", "--compile"}, "compiles the program", 0},
				{"debug", vector<string>{"-d", "--debug"}, "enables debug options", 0},
				{"help", vector<string>{"-h", "--help"}, "shows this help", 0},
				{"locate", vector<string>{"-l", "--locate"}, "shows executable location", 0},
		};
		argagg::parser argparser{options};
		argagg::parser_results arguments = argparser.parse(argc, argv);
		debug = arguments["debug"];
		bool showHelp = arguments["help"] || arguments.pos.empty();
		if (debug || showHelp) {
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
			PoolVM::initialiaze({debug, args});
			if (arguments["compile"]) {
				PoolVM::get()->compile(filename);
			} else {
				PoolVM::get()->execute(filename);
			}
			if (debug) {
				auto executionTime = getExecutionTime(startTime, high_resolution_clock::now());
				cerr << endl << termcolor::bright_green << "Execution successful" << termcolor::reset;
				cerr << " in " << executionTime << endl << endl;
			}
			return EXIT_SUCCESS;
		}
	} catch (const compile_error &e) {
		cerr << e << endl;
		if (debug) {
			auto executionTime = getExecutionTime(startTime, high_resolution_clock::now());
			cerr << endl << termcolor::red << "Execution failed" << termcolor::reset;
			cerr << " in " << executionTime << endl << endl;
		}
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

