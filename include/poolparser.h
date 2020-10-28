#pragma once

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <new>

namespace pool::parser {
	extern "C" const char *generate_json_ast(const char *file);
}
