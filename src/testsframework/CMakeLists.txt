set(LIBS ${LIBS} Eridani-Lib)

file(GLOB_RECURSE SOURCES "api/*.h" "main/*.h" "main/*.c")
add_library(Eridani-Testing-Framework STATIC ${SOURCES})
target_link_libraries(Eridani-Testing-Framework ${LIBS})
target_include_directories(Eridani-Testing-Framework PRIVATE main/)
target_include_directories(Eridani-Testing-Framework PUBLIC api/)
target_include_directories(Eridani-Testing-Framework PRIVATE ${DIR_LIB}/api ${DIR_LIB}/main)
set_target_properties(Eridani-Testing-Framework PROPERTIES
	VERSION ${PROJECT_VERSION}
	)

install(TARGETS Eridani-Testing-Framework
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
