add_library(aether_engine_ecs STATIC
    EntityManager.cpp
)

target_include_directories(aether_engine_ecs PRIVATE ${ENGINE_INCLUDE_DIR})

target_link_libraries(aether_engine_ecs PRIVATE
    aether_engine_build
)

add_library(Aether::Engine::ECS ALIAS aether_engine_ecs)
