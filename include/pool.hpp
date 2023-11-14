#pragma once

#include <util/macro.hpp>
#include <vector>
#include <memory>
#include <string>

using namespace std;

namespace pool {
	class PoolInstance {
	public:
		const string path;
		const string directory;

		explicit PoolInstance(const string &path);

		virtual void compile() noexcept(false) = 0;

		POOL_PUBLIC static shared_ptr<PoolInstance> load(const string &path);
	};

	class PoolVM {
	public:
		struct Settings {
			bool debug;
			vector<string> args;
		};
		constexpr static const string_view EXT = ".pool";

		virtual void compile(const string &module) noexcept(false) = 0;

		POOL_PUBLIC static void initialiaze(const Settings &settings);

		POOL_PUBLIC static shared_ptr<PoolVM> get();

		POOL_PUBLIC static string getSDKPath();

		POOL_PUBLIC static string getVersion();
	};
}
