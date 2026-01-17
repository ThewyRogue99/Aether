add_library(aether_engine SHARED Engine.cpp)

add_library(aether_engine_build INTERFACE)

target_compile_options(aether_engine_build INTERFACE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-fno-rtti>
    $<$<CXX_COMPILER_ID:MSVC>:/GR->
)

set(ENGINE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Core)
add_subdirectory(Platform)
add_subdirectory(Log)
add_subdirectory(Math)
add_subdirectory(Input)
add_subdirectory(Events)
add_subdirectory(Renderer)

target_include_directories(aether_engine PUBLIC
    ${ENGINE_INCLUDE_DIR}
    ${ENGINE_SOURCE_DIR}
)

target_link_libraries(aether_engine
    PRIVATE
        aether_engine_build
    PUBLIC
        Aether::Engine::Core
        Aether::Engine::Log
        Aether::Engine::Platform
        Aether::Engine::Input
        Aether::Engine::Math
        Aether::Engine::Events
        Aether::Engine::Renderer
)

add_library(Aether::Engine ALIAS aether_engine)
