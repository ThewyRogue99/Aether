cmake_minimum_required(VERSION 4.0)
project(Aether)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(AETHER_BUILD_DEBUG)
else()
    add_compile_definitions(AETHER_BUILD_RELEASE)
endif()

add_subdirectory(external)
add_subdirectory(src)