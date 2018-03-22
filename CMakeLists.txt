cmake_minimum_required (VERSION 3.1)
project (LOOP-Interpreter)
set (LOOP-Interpreter_VERSION_MAJOR 1)
set (LOOP-Interpreter_VERSION_MINOR 0)

include_directories (include)
file (GLOB SOURCES "src/*.c")

add_executable (loop ${SOURCES})
set_property (TARGET loop PROPERTY C_STANDARD 99)
