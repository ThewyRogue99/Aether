add_library(aether_engine_systems STATIC
    Movement/MovementSystem.cpp
    Rendering/RenderingSystem.cpp
)

target_include_directories(aether_engine_systems PRIVATE ${ENGINE_INCLUDE_DIR} ${ENGINE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(aether_engine_systems PRIVATE
    aether_engine_build
)

add_library(Aether::Engine::Systems ALIAS aether_engine_systems)
