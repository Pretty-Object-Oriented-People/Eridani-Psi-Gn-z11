set(LIBS ${LIBS} Eridani-Testing-Framework)
set(LIBS ${LIBS} Eridani-Lib)
set(LIBS ${LIBS} json-c)

file(GLOB_RECURSE SOURCES "*.h" "*.c")
add_executable(Eridani-Lib-Test ${SOURCES})
target_include_directories(Eridani-Testing-Framework PRIVATE ./)
target_link_libraries(Eridani-Lib-Test ${LIBS})

target_include_directories(Eridani-Lib-Test PRIVATE ${DIR_TESTSFRAMEWORK}/api)
target_include_directories(Eridani-Lib-Test PRIVATE ../api/ ../main)
