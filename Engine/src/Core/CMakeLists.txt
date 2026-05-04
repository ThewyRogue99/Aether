add_library(aether_engine_core STATIC
    Application.cpp
    StringView.cpp
    String.cpp
    StringBuilder.cpp
    RuntimeObject.cpp
    Hash.cpp
    Name.cpp
    Layer.cpp
    LayerStack.cpp
    Json.cpp
)

target_include_directories(aether_engine_core PRIVATE ${ENGINE_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(aether_engine_core PRIVATE
    aether_engine_build
    Aether::Engine::Platform
    nlohmann_json::nlohmann_json
)

add_subdirectory(Memory)

add_library(Aether::Engine::Core ALIAS aether_engine_core)
