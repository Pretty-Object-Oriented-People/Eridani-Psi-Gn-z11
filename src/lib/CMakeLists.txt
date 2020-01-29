file(GLOB SOURCES "api/*.h" "main/*.h" "main/*.c")
add_library(Eridani-Lib STATIC ${SOURCES})
set_target_properties(Eridani-Lib PROPERTIES
	VERSION ${PROJECT_VERSION}
	)

install(TARGETS Eridani-Lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

add_subdirectory(test)