#include "dylib.hpp"
#include "strings.hpp"
#include <utility>

#if defined(__linux__) || defined(__linux) || defined(linux) || defined(_LINUX)
#define ON_LINUX 1
#define ON_WINDOWS 0
#define ON_MACOS 0
#elif defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)
#define ON_LINUX 0
#define ON_WINDOWS 1
#define ON_MACOS 0
#elif defined(__APPLE__)
#define ON_LINUX 0
#define ON_WINDOWS 0
#define ON_MACOS 1
#else
#error "OS Not Supported"
#endif

#if ON_WINDOWS

#include <windows.h>
#include <strsafe.h>
#include <AtlBase.h>
#include <atlconv.h>

typedef HMODULE Handle;
#endif

#if ON_LINUX || ON_MACOS

#include <dlfcn.h>

typedef void *Handle;
#endif

string getLastError() {
#if ON_LINUX || ON_MACOS
	return string(::dlerror());
#endif
#if ON_WINDOWS
	// https://msdn.microsoft.com/en-us/library/ms680582%28VS.85%29.aspx
	LPVOID lpMsgBuf;
	DWORD dw = ::GetLastError();
	::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, nullptr);
	auto lpDisplayBuf = (LPVOID) ::LocalAlloc(LMEM_ZEROINIT, (::lstrlen((LPCTSTR) lpMsgBuf) + 40) * sizeof(TCHAR));
	::StringCchPrintf((LPTSTR) lpDisplayBuf,
					  ::LocalSize(lpDisplayBuf) / sizeof(TCHAR),
					  TEXT("Failed with error %d: %s"),
					  dw, lpMsgBuf);
	wstring err_str((LPCTSTR) lpDisplayBuf);
	CW2A cw2a(err_str.c_str());
	::LocalFree(lpMsgBuf);
	::LocalFree(lpDisplayBuf);
	return string(cw2a);
#endif
}

bool close(const Handle &handle) {
#if ON_WINDOWS
	const BOOL rc = ::FreeLibrary(handle);
	if (rc == 0) { // https://msdn.microsoft.com/en-us/library/windows/desktop/ms683152(v=vs.85).aspx
		return false;
	}
#endif
#if ON_LINUX || ON_MACOS
	const int rc = ::dlclose(handle);
	if (rc != 0) {
		return false;
	}
#endif
	return true;
}

struct DynamicLibrariesManager::Library {
	string path;
	Handle handle;

	Library(string path, const Handle &handle) : path(std::move(path)), handle(handle) {}

	~Library() noexcept(false) {
		if (!close(handle)) {
			throw runtime_error("Failed to close dynamic library: " + getLastError());
		}
	}
};

vector<unique_ptr<DynamicLibrariesManager::Library>> DynamicLibrariesManager::libraries;

void DynamicLibrariesManager::load(const string &name) {
	auto path = fs::path(name);
#if ON_WINDOWS
	if (!path.has_extension()) {
		path.replace_extension("dll");
	}
#endif
#if ON_LINUX || ON_MACOS
	if (!startsWith(path.filename().string(), "lib")) {
		path.replace_filename("lib" + path.filename().string());
	}
#endif
#if ON_MACOS
	if (!path.has_extension()) {
		path.replace_extension("bundle");
	}
#endif
#if ON_LINUX
	if (!path.has_extension()) {
		path.replace_extension("so");
	}
#endif
	load(path);
}

void DynamicLibrariesManager::loadPath(const string &path) {
	load(fs::path(path));
}

void DynamicLibrariesManager::load(const fs::path &path) {
	if (!fs::exists(path))
		throw runtime_error("'" + path.string() + "' does not exist");
	if (!fs::is_regular_file(path))
		throw runtime_error("'" + path.string() + "' is not a regular file");
	string file = fs::canonical(path).string();
#if ON_WINDOWS
	CA2W ca2w(file.c_str());
	Handle handle = LoadLibrary(ca2w);
#endif
#if ON_LINUX || ON_MACOS
	Handle handle = ::dlopen(file.c_str(), RTLD_NOW);
#endif
	if (handle == nullptr) {
		throw runtime_error("Failed to open '" + file + "': " + getLastError());
	}
	libraries.push_back(make_unique<Library>(file, handle));
}
