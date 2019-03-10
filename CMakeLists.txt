cmake_minimum_required(VERSION 3.0.0)
project(coding_problems VERSION 0.1.0)

if (NOT WIN32)
    set(CMAKE_EXECUTABLE_SUFFIX_CXX ".out" CACHE STRING "" FORCE)
    set(CMAKE_EXECUTABLE_SUFFIX_C   ".out" CACHE STRING "" FORCE)
endif()

add_subdirectory(pe)