#include <iostream>
#include "poolx.hpp"
#include "../lib/argagg.hpp"

int main(int argc, char **argv) {
	try {
		vector<argagg::definition> options{
				{"debug", {"-d", "--debug"}, "enables debug options", 0}
		};
		argagg::parser argparser{options};
		argagg::parser_results result = argparser.parse(argc, argv);
		if (!result.pos.empty()) {
			string file = result.pos[0];
			vector<string> args;
			for (int i = 1; i < result.pos.size(); ++i) {
				args.emplace_back(result.pos[i]);
			}
			auto app = pool::poolx::load(file, result["debug"]);
			app->execute(args);
		} else {
			std::cout << "poolx " << pool::poolx::VERSION << std::endl;
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}

