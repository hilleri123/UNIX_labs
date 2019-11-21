
cmake_minimum_required (VERSION 3.7)

project (myProject VERSION 1.00 LANGUAGES CXX)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set (CMAKE_VERBOSE_MAKEFILE on)


include_directories (.)	#Сиська
include_directories (files)

link_directories (files)

set (SOURCES 
	files/main.cpp files/functions
	#files/push.c
	#files/pull.c
	#files/create.c
	#files/delete.c
	#files/status.c
	)


add_subdirectory (files)

add_executable (${PROJECT_NAME} ${SOURCES})

target_link_libraries (${PROJECT_NAME} lib)
