#include "poolx.hpp"

int main(int argc, char **argv) {
	if (argc >= 2) {
		string file = argv[1];
		vector<string> args;
		for (int i = 2; i < argc; ++i) {
			args.emplace_back(argv[i]);
		}
		poolx::execute(file, args);
	}
}

