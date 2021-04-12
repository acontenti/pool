#pragma once

#include <util/macro.hpp>
#include <vector>
#include <memory>
#include <string>

using namespace std;

namespace pool {
	class POOL_PUBLIC PoolInstance {
	public:
		const string path;

		PoolInstance(string path);
	};

	class POOL_PUBLIC PoolVM {
	public:
		struct Settings {
			bool debug;
			vector<string> args;
		};
		constexpr static const string_view VERSION = POOL_VERSION;
		constexpr static const string_view EXT = ".pool";

		static void initialiaze(const Settings &settings);

		virtual void execute(const string &module) noexcept(false) = 0;

		static shared_ptr<PoolVM> get();
	};
}