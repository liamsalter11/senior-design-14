cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)

project(mainTests VERSION 1.0)

set(MAIN_SOURCES
	main/src/LiFiCompression.cpp
	main/src/LiFiCorrection.cpp
	test/src/LiFiSimulation.cpp)
	
set(BITSET_SOURCES
	main/src/Bitset.cpp)
	
set(TREE_SOURCES
	research/src/HuffmanTree.cpp)
	
set(MAIN_INCLUDES
	main/inc
	test/inc)
	
set(BITSET_INCLUDES
	${MAIN_INCLUDES})
	
set(TREE_INCLUDES
	${MAIN_INCLUDES})
	
set(SYNC_INCLUDES
	${MAIN_INCLUDES})
	
add_library(bitsetLib ${BITSET_SOURCES})
target_include_directories(bitsetLib PRIVATE ${BITSET_INCLUDES})
	
add_library(mainLib ${MAIN_SOURCES})
target_include_directories(mainLib PRIVATE ${MAIN_INCLUDES})
target_link_libraries(mainLib bitsetLib)
	
add_executable(mainTests test/src/fullTest.cpp)
target_include_directories(mainTests PRIVATE ${MAIN_INCLUDES})
target_link_libraries(mainTests mainLib)

add_executable(bitsetTests test/src/bitsetTests.cpp)
target_include_directories(bitsetTests PRIVATE ${BITSET_INCLUDES})
target_link_libraries(bitsetTests bitsetLib)

add_executable(treeTests test/src/treeTests.cpp ${TREE_SOURCES})
target_include_directories(treeTests PRIVATE ${TREE_INCLUDES})
target_link_libraries(treeTests mainLib)

add_executable(syncTests test/src/syncTests.cpp)
target_include_directories(syncTests PRIVATE ${SYNC_INCLUDES})
target_link_libraries(syncTests mainLib)
	
	