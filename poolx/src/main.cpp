#include "poolx.hpp"

int main(int argc, char **argv) {
	if (argc == 2) {
		std::string file = argv[1];
		poolx::execute(file);
	}
}

