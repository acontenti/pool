cmake_minimum_required(VERSION 3.20)
project(pool VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# LLVM
find_package(LLVM 17 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# ANTLR4 RUNTIME
set(ANTLR4_VERSION "4.13.1")
add_compile_definitions(_HAS_STD_BYTE=0)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
add_definitions(-DANTLR4CPP_STATIC)
set(ANTLR4_WITH_STATIC_CRT OFF)
set(ANTLR4_TAG "${ANTLR4_VERSION}")
include(ExternalAntlr4Cpp)
set(ANTLR4_LIB antlr4_static)

# ANTLR4 JAVA
set(ANTLR_EXECUTABLE ${CMAKE_SOURCE_DIR}/antlr4/antlr-${ANTLR4_VERSION}-complete.jar)
find_package(ANTLR REQUIRED)

# PARSER GENERATOR
antlr_target(PoolLexer PoolLexer.g4 LEXER OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/gen)
antlr_target(PoolParser PoolParser.g4 PARSER OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/gen DEPENDS_ANTLR PoolLexer)

# POOL
file(GLOB_RECURSE POOL_SRC src/*.cpp)
add_executable(pool)
target_sources(pool PRIVATE ${POOL_SRC})
target_sources(pool PRIVATE ${ANTLR_PoolLexer_CXX_OUTPUTS} ${ANTLR_PoolParser_CXX_OUTPUTS})
target_include_directories(pool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(pool PRIVATE ${ANTLR_PoolLexer_OUTPUT_DIR} ${ANTLR_PoolParser_OUTPUT_DIR} ${ANTLR4_INCLUDE_DIRS})
target_include_directories(pool PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(pool PUBLIC POOL_VERSION="${PROJECT_VERSION}" POOL_EXPORTS)
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
target_compile_definitions(pool PUBLIC ${LLVM_DEFINITIONS_LIST})
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
llvm_map_components_to_libnames(llvm_libs support core irreader mc nativecodegen)
target_link_libraries(pool ${llvm_libs})

# POOLSTDLIB
add_library(poolstd SHARED)
target_sources(poolstd PRIVATE src/natives.cpp src/poolstd.cpp)
target_include_directories(poolstd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(poolstd PUBLIC POOL_VERSION="${PROJECT_VERSION}" POOL_EXPORTS POOLSTDLIB)
set_target_properties(
		poolstd PROPERTIES
		ENABLE_EXPORTS ON
		POSITION_INDEPENDENT_CODE ON
		CXX_STANDARD 17
		CXX_VISIBILITY_PRESET hidden
)

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
