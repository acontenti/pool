#pragma once

#include <vector>
#include <memory>
#include <string>

using namespace std;

namespace pool {
	struct Settings {
		bool debug;
		const vector<string> &args;
	};

	class Pool {
		bool result;

		Pool(const string &filename, istream &stream);

	public:
		[[nodiscard]] constexpr inline bool getResult() const {
			return result;
		}

		constexpr static const string_view VERSION = POOL_VERSION;
		constexpr static const string_view EXT = ".pool";

		static void initialiaze(const Settings &settings);

		static Pool execute(const string &filename);
	};
}