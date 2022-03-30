cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)

project(mainTests VERSION 1.0)

set(SOURCES
	source/Bitset.cpp
	source/LiFiCompression.cpp
	source/LiFiCorrection.cpp
	source/HuffmanTree.cpp)
	
set(TEST_SOURCES
	tests/LiFiSimulation.cpp)
	
add_executable(mainTests tests/fullTest.cpp ${TEST_SOURCES} ${SOURCES})
target_include_directories(mainTests PRIVATE include tests)

add_executable(bitsetTests tests/bitsetTests.cpp ${TEST_SOURCES} ${SOURCES})
target_include_directories(bitsetTests PRIVATE include)

add_executable(treeTests tests/treeTests.cpp ${TEST_SOURCES} ${SOURCES})
target_include_directories(treeTests PRIVATE include)
	
	