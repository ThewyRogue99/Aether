add_library(AetherEngineCore STATIC
    Application.cpp
    StringView.cpp
    String.cpp
    StringBuilder.cpp
    RuntimeObject.cpp
    Hash.cpp
    Name.cpp
)

target_include_directories(AetherEngineCore PRIVATE ${ENGINE_INCLUDE_DIR})

target_link_libraries(AetherEngineCore PRIVATE AetherEngineBuild)

add_subdirectory(Memory)
