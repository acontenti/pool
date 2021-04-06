#pragma once

#include <vector>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

class DynamicLibrariesManager {
	struct Library;
	static vector<unique_ptr<Library>> libraries;

	static void load(const fs::path &path);

public:

	static void load(const string& name);

	static void loadPath(const string& path);
};
