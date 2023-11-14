#pragma once

#ifdef _WIN32
#ifdef POOL_EXPORTS
#define POOL_PUBLIC __declspec(dllexport)
#else
#define POOL_PUBLIC __declspec(dllimport)
#endif
#if defined(_MSC_VER) && !defined(__clang__)
// clang-cl should escape this to prevent [ignored-attributes].
namespace std {
	class POOL_PUBLIC exception; // Prevents warning C4275 from MSVC.
} // namespace std
#endif
#elif defined(__APPLE__)
#if __GNUC__ >= 4
#define POOL_PUBLIC __attribute__ ((visibility ("default")))
#else
#define POOL_PUBLIC
#endif
#else
#if __GNUC__ >= 4
#define POOL_PUBLIC __attribute__ ((visibility ("default")))
#else
#define POOL_PUBLIC
#endif
#endif

#define EXTERN extern "C" [[maybe_unused]]
