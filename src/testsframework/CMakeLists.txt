file(GLOB SOURCES "api/*.h" "main/*.h" "main/*.c")
add_library(Eridani-Testing-Framework STATIC ${SOURCES})
set_target_properties(Eridani-Testing-Framework PROPERTIES
	VERSION ${PROJECT_VERSION}
	)

install(TARGETS Eridani-Testing-Framework
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
