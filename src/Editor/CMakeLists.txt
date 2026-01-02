find_package(OpenGL REQUIRED)

add_library(AetherEditor src/EditorApp.cpp)

add_subdirectory(src)

target_link_libraries(AetherEditor PRIVATE
    Aether::Engine
)

target_include_directories(AetherEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(Aether::Editor ALIAS AetherEditor)

add_executable(AetherEditorApp src/main.cpp)
target_link_libraries(AetherEditorApp PUBLIC Aether::Editor imgui glfw OpenGL::GL)
