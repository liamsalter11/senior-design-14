cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)

project(mainTests VERSION 1.0)

set(SOURCES
	source/Bitset.cpp
	source/LiFiCompression.cpp
	source/LiFiCorrection.cpp)
	
set(TEST_SOURCES
	tests/LiFiSimulation.cpp)
	
add_executable(mainTest tests/fullTest.cpp ${TEST_SOURCES} ${SOURCES})
target_include_directories(mainTest PRIVATE include tests)
	
	