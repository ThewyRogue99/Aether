add_library(aether_engine SHARED Engine.cpp)

add_library(aether_engine_build INTERFACE)

target_compile_options(aether_engine_build INTERFACE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-fno-rtti>
    $<$<CXX_COMPILER_ID:MSVC>:/GR->
)

set(ENGINE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Core)
add_subdirectory(Platform)

target_include_directories(aether_engine PUBLIC
    ${ENGINE_INCLUDE_DIR}
    ${ENGINE_SOURCE_DIR}
)

target_link_libraries(aether_engine PRIVATE
    aether_engine_build
    Aether::Engine::Core
    Aether::Engine::Platform
)

add_library(Aether::Engine ALIAS aether_engine)
