add_executable(Editor
    EditorApp.cpp
    EditorLayer.cpp
    EditorConfig.cpp
    EditorRenderingSystem.cpp
    EditorFonts.cpp
    Panels/ViewportPanel.cpp
    Panels/SceneHierarchyPanel.cpp
    Panels/PropertiesPanel.cpp
    Themes/ThemeRegistry.cpp
)

set(EDITOR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(Editor PUBLIC ${EDITOR_SOURCE_DIR})

target_link_libraries(Editor PUBLIC Aether::Engine imgui)

# Copy the editor's assets (fonts, etc.) next to the binary so relative
# paths like "assets/fonts/Inter-Regular.ttf" resolve at runtime.
add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/../assets
            $<TARGET_FILE_DIR:Editor>/assets
    COMMENT "Copying Editor assets next to binary"
)
