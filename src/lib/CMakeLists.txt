set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(LIBS ${LIBS} ${CMAKE_THREAD_LIBS_INIT})

file(GLOB_RECURSE SOURCES "api/*.h" "main/*.hpp" "main/*.h" "main/*.cpp" "main/*.c")
add_library(Eridani-Lib STATIC ${SOURCES})
target_link_libraries(Eridani-Lib ${LIBS})
target_include_directories(Eridani-Lib PRIVATE main/)
target_include_directories(Eridani-Lib PUBLIC api/)
set_target_properties(Eridani-Lib PROPERTIES
	VERSION ${PROJECT_VERSION}
	)

install(TARGETS Eridani-Lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

add_subdirectory(test)