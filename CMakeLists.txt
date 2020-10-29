cmake_minimum_required(VERSION 3.10)
project(pool VERSION 0.1.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
add_compile_definitions(POOL_VERSION="${PROJECT_VERSION}")
enable_language(Rust)
include(CMakeCargo)

include_directories(include)
add_subdirectory(poolparser)

file(GLOB_RECURSE POOLX_SRC poolx/src/*.cpp)
add_executable(poolx ${POOLX_SRC})
set_property(TARGET poolx PROPERTY CXX_STANDARD 17)
if (WIN32)
	target_link_libraries(poolx poolparser ws2_32 userenv)
else ()
	target_link_libraries(poolx poolparser dl pthread)
endif ()
configure_file(test.pool ${CMAKE_BINARY_DIR}/test.pool COPYONLY)

# INSTALL
install(TARGETS poolx CONFIGURATIONS Release DESTINATION ${CMAKE_SOURCE_DIR}/dist)

# TEST
enable_testing()
add_test(NAME basic_test COMMAND poolx ${CMAKE_SOURCE_DIR}/test.pool)