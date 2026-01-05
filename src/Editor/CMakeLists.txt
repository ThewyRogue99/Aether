add_library(AetherEditor src/EditorApp.cpp)

add_subdirectory(src)

target_link_libraries(AetherEditor PUBLIC
    Aether::Engine
)

add_library(Aether::Editor ALIAS AetherEditor)

add_executable(AetherEditorApp src/main.cpp)
target_link_libraries(AetherEditorApp PUBLIC Aether::Editor)
