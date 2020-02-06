set(LIBS ${LIBS} Eridani-Lib)
set(LIBS ${LIBS} json-c)

file(GLOB_RECURSE SOURCES "*.hpp" "*.cpp" "*.h" "*.c")
add_executable(Eridani-Genofun ${SOURCES})
target_link_libraries(Eridani-Genofun ${LIBS})
target_include_directories(Eridani-Genofun PRIVATE ${DIR_LIB}/api ${DIR_LIB}/main)
