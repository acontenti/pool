cmake_minimum_required(VERSION 3.10)
project(pool-external-test LANGUAGES CXX)

add_library(pool-external-test MODULE main.cpp)
set_target_properties(
		pool-external-test PROPERTIES
		DEBUG_POSTFIX ""
		CXX_STANDARD 17
)
target_include_directories(pool-external-test PUBLIC ../../include)
if(MSVC)
	target_link_libraries(pool-external-test PUBLIC ../../pool)
else()
	target_link_libraries(pool-external-test PUBLIC pool)
endif()
if(APPLE)
	set_target_properties(pool-external-test PROPERTIES SUFFIX ".bundle")
endif()
configure_file(etest.pool ${CMAKE_CURRENT_BINARY_DIR}/etest.pool COPYONLY)