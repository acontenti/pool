cmake_minimum_required(VERSION 3.20)
project(pool VERSION 0.2.0 LANGUAGES CXX)

# CPPLOCATE
include(FetchContent)
SET(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libs")
FetchContent_Declare(cpplocate GIT_REPOSITORY https://github.com/cginternals/cpplocate.git GIT_TAG v2.2.0)
FetchContent_GetProperties(cpplocate)
if (NOT cpplocate_POPULATED)
	FetchContent_Populate(cpplocate)
	add_subdirectory(${cpplocate_SOURCE_DIR} ${cpplocate_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()
find_package(cpplocate REQUIRED)

# ANTLR4 RUNTIME
set(ANTLR4_VERSION "4.10.1")
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

# POOL
include_directories(${ANTLR_PoolLexer_OUTPUT_DIR} ${ANTLR_PoolParser_OUTPUT_DIR} ${ANTLR4_INCLUDE_DIRS})
file(GLOB_RECURSE POOL_SRC src/*.cpp)
add_executable(pool ${POOL_SRC} ${ANTLR_PoolLexer_CXX_OUTPUTS} ${ANTLR_PoolParser_CXX_OUTPUTS})
target_include_directories(pool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(pool PUBLIC POOL_VERSION="${PROJECT_VERSION}" POOL_EXPORTS)
set_target_properties(
		pool PROPERTIES
		ENABLE_EXPORTS ON
		POSITION_INDEPENDENT_CODE ON
		CXX_STANDARD 17
		CXX_VISIBILITY_PRESET hidden
)
find_package(Threads REQUIRED)
target_link_libraries(pool Threads::Threads)
target_link_libraries(pool ${ANTLR4_LIB})
target_link_libraries(pool cpplocate::cpplocate)

# COPY STD TO BINARY DIR
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/std/std.pool ${CMAKE_CURRENT_BINARY_DIR}/std/std.pool COPYONLY)

# INSTALL
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist)
set(INSTALL_PATH ${CMAKE_SOURCE_DIR}/dist)
install(TARGETS pool CONFIGURATIONS Release DESTINATION ${INSTALL_PATH})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/std/ DESTINATION ${INSTALL_PATH}/std)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${INSTALL_PATH}/include)

# TEST
add_subdirectory(test/external-test)

enable_testing()
add_test(NAME run_test COMMAND pool)
add_test(NAME locate_test COMMAND pool --locate)
cmake_path(NATIVE_PATH CMAKE_BINARY_DIR EXPECTED_DIR)
string(REPLACE "\\" "\\\\" EXPECTED_DIR ${EXPECTED_DIR})
set_tests_properties(locate_test PROPERTIES PASS_REGULAR_EXPRESSION ${EXPECTED_DIR})
add_test(NAME basic_test COMMAND pool test ctest WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)
add_test(NAME external_test COMMAND pool external-test/etest.pool WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test)