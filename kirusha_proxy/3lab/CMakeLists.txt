
cmake_minimum_required (VERSION 3.7)

project (mySwamp VERSION 1.00 LANGUAGES CXX)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pthread")

set (CMAKE_VERBOSE_MAKEFILE on)


#target_link_libraries(${PROJECT_NAME} pthread)

set (SOURCES 
	main.cpp
	swamp.cpp swamp.h
	creature.cpp creature.h
	)



add_executable (${PROJECT_NAME} ${SOURCES})

