#include <iostream>
#include "pool.hpp"
#include "../lib/argagg.hpp"

using namespace pool;

int main(int argc, char **argv) {
	try {
		vector<argagg::definition> options{
				{"debug", vector<string>{"-d", "--debug"}, "enables debug options", 0}
		};
		argagg::parser argparser{options};
		argagg::parser_results result = argparser.parse(argc, argv);
		bool debug = result["debug"];
		if (debug || result.pos.empty()) {
			cout << "pool " << pool::Pool::VERSION << endl;
		}
		if (!result.pos.empty()) {
			vector<string> args;
			string filename = result.pos[0];
			for (int i = 1; i < result.pos.size(); ++i) {
				args.emplace_back(result.pos[i]);
			}
			Pool::setOptions({debug, args});
			Pool::execute(filename);
		}
		return EXIT_SUCCESS;
	} catch (const exception &e) {
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	}
}

