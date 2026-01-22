add_library(aether_engine_scene STATIC
    Camera.cpp
)

target_include_directories(aether_engine_scene PRIVATE ${ENGINE_INCLUDE_DIR} ${ENGINE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(aether_engine_scene PRIVATE
    aether_engine_build
    Aether::Engine::Math
    glm::glm
)

add_library(Aether::Engine::Scene ALIAS aether_engine_scene)
