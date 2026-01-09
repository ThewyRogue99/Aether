add_library(aether_engine_input STATIC
    Input.cpp
)

target_include_directories(aether_engine_input PRIVATE ${ENGINE_INCLUDE_DIR})

target_link_libraries(aether_engine_input PRIVATE
    aether_engine_build
)

add_library(Aether::Engine::Input ALIAS aether_engine_input)
