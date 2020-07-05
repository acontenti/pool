cmake_minimum_required(VERSION 3.10)
project(pool VERSION 0.1.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
enable_language(Rust)
include(CMakeCargo)

include_directories(include)
add_subdirectory(poolc)

file(GLOB_RECURSE POOLX_SRC poolx/src/*.cpp)
add_executable(poolx ${POOLX_SRC})
target_compile_features(poolx PUBLIC cxx_std_17)
if(WIN32)
	target_link_libraries(poolx libpool ws2_32 userenv)
else()
	target_link_libraries(poolx libpool dl pthread)
endif()
configure_file(test.pool ${CMAKE_BINARY_DIR}/test.pool COPYONLY)