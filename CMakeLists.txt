cmake_minimum_required(VERSION 3.10)
project(Eridani VERSION 0.0.1 DESCRIPTION "Eridani Organization Helper" LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(DIR_TESTSFRAMEWORK ${CMAKE_CURRENT_SOURCE_DIR}/src/testsframework)
set(DIR_LIB ${CMAKE_CURRENT_SOURCE_DIR}/src/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/)
if (WIN32)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/Windows/)
endif (WIN32)
if (UNIX)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/Linux/)
endif (UNIX)

add_subdirectory(${DIR_TESTSFRAMEWORK})
add_subdirectory(${DIR_LIB})

enable_testing()
add_test(NAME LibTest COMMAND Eridani-Lib-Test)

