cmake_minimum_required(VERSION 3.10)
project(pool VERSION 0.1.0 LANGUAGES CXX)

# ANTLR4 RUNTIME
set(ANTLR4_VERSION "4.9.2")
add_compile_definitions(_HAS_STD_BYTE=0)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
if (ANTLR4_BASE_DIR)
	# ANTLR4 runtime as external project - define:
	# -DANTLR4_BASE_DIR=<project root path>
	# -DANTLR4_LIB_NAME=<library file name without path>
	set(ANTLR4_INCLUDE_DIRS ${ANTLR4_BASE_DIR}/runtime/src)
	set(ANTLR4_LIB ${ANTLR4_BASE_DIR}/dist/${ANTLR4_LIB_NAME})
else ()
	# download and build ANTLR4 runtime in project
	add_definitions(-DANTLR4CPP_STATIC)
	set(ANTLR4_WITH_STATIC_CRT OFF)
	set(ANTLR4_TAG "${ANTLR4_VERSION}")
	include(ExternalAntlr4Cpp)
	set(ANTLR4_LIB antlr4_static)
endif ()

# ANTLR4 JAVA
set(ANTLR_EXECUTABLE ${CMAKE_SOURCE_DIR}/antlr4/antlr-${ANTLR4_VERSION}-complete.jar)
find_package(ANTLR REQUIRED)

# PARSER GENERATOR
antlr_target(PoolLexer PoolLexer.g4 LEXER OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/gen)
antlr_target(PoolParser PoolParser.g4 PARSER OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/gen DEPENDS_ANTLR PoolLexer)

# POOL STD RESOURCES
add_custom_target(embed_std COMMAND ${CMAKE_COMMAND} -Dsource_dir="${CMAKE_SOURCE_DIR}/std" -Doutput_dir="${CMAKE_SOURCE_DIR}/src" -P ${CMAKE_SOURCE_DIR}/cmake/ResourceEmbed.cmake)

# POOL
include_directories(${ANTLR_PoolLexer_OUTPUT_DIR} ${ANTLR_PoolParser_OUTPUT_DIR} ${ANTLR4_INCLUDE_DIRS})
add_compile_definitions(POOL_VERSION="${PROJECT_VERSION}")
file(GLOB_RECURSE POOL_SRC src/*.cpp)
add_executable(pool ${POOL_SRC} ${ANTLR_PoolLexer_CXX_OUTPUTS} ${ANTLR_PoolParser_CXX_OUTPUTS})
set_property(TARGET pool PROPERTY CXX_STANDARD 17)
if (WIN32)
	target_link_libraries(pool ws2_32 userenv)
else ()
	target_link_libraries(pool dl pthread stdc++fs)
endif ()
target_link_libraries(pool ${ANTLR4_LIB})
add_dependencies(pool embed_std)

# INSTALL
install(TARGETS pool CONFIGURATIONS Release DESTINATION ${CMAKE_SOURCE_DIR}/dist)

# TEST
enable_testing()
add_test(NAME basic_test COMMAND pool test ctest WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)