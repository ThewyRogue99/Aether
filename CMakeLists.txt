cmake_minimum_required(VERSION 3.16)
project(Aether LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(AETHER_BUILD_DEBUG)
else()
    add_compile_definitions(AETHER_BUILD_RELEASE)
endif()

add_subdirectory(external)
add_subdirectory(src)