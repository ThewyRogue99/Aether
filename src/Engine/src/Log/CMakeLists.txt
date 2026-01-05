add_library(aether_engine_log STATIC
    Log.cpp
    Logger.cpp
    StdColorSink.cpp
    NullSink.cpp
)

target_include_directories(aether_engine_log PRIVATE ${ENGINE_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(aether_engine_log PRIVATE
    aether_engine_build
    spdlog::spdlog
)

add_library(Aether::Engine::Log ALIAS aether_engine_log)
