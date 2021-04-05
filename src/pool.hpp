#pragma once

#include <vector>
#include <memory>
#include <string>

using namespace std;

namespace pool {

	class PoolVM {
		class PoolInstance;

		static struct Settings {
			bool debug;
			vector<string> args;
		} settings;
		static vector<shared_ptr<PoolInstance>> instances;
	public:
		constexpr static const string_view VERSION = POOL_VERSION;
		constexpr static const string_view EXT = ".pool";

		static void initialiaze(const Settings &settings);

		static bool execute(const string &filename);
	};

	extern void initializeStdLib();
}