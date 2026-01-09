add_library(aether_engine_events STATIC
    EventQueue.cpp
)

target_include_directories(aether_engine_events PRIVATE ${ENGINE_INCLUDE_DIR})

target_link_libraries(aether_engine_events PRIVATE
    aether_engine_build
)

add_library(Aether::Engine::Events ALIAS aether_engine_events)
