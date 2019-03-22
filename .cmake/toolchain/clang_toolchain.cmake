set(CMAKE_C_COMPILER    "clang" CACHE STRING "" FORCE)
set(CMAKE_C_COMPILER_ID "clang" CACHE STRING "" FORCE)

set(CMAKE_CXX_COMPILER    "clang++" CACHE STRING "" FORCE)
set(CMAKE_CXX_COMPILER_ID "clang"   CACHE STRING "" FORCE)

set(CMAKE_LINKER "lld" CACHE STRING "" FORCE)

if (NOT WIN32)
    set(CMAKE_EXECUTABLE_SUFFIX_CXX ".out" CACHE STRING "" FORCE)
    set(CMAKE_EXECUTABLE_SUFFIX_C   ".out" CACHE STRING "" FORCE)
endif(NOT WIN32)

add_compile_options("SHELL:-Weverything -pedantic -fcolor-diagnostics")
add_compile_options("SHELL:-Wno-c++98-compat -Wno-c++98-compat-pedantic")
