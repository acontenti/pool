#pragma once

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <new>

namespace libpool {

extern "C" {

const char *generate_json_ast(const char *file);

} // extern "C"

} // namespace libpool
