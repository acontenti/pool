#pragma once

#include <vector>
#include <memory>

using namespace std;

namespace pool {
	struct Settings {
		bool debug;
		const vector<string> &args;
	};

	class PoolX {
		explicit PoolX(const string &file);

	public:
		constexpr static const string_view VERSION = POOL_VERSION;
		constexpr static const string_view EXT = ".pool";

		static void setOptions(const Settings &settings);

		static PoolX execute(const string &filename);
	};
}