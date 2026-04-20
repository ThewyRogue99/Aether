add_executable(Editor
    EditorApp.cpp
    EditorLayer.cpp
    Panels/ViewportPanel.cpp
    Panels/SceneHierarchyPanel.cpp
    Panels/PropertiesPanel.cpp
)

set(EDITOR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(Editor PUBLIC ${EDITOR_SOURCE_DIR})

target_link_libraries(Editor PUBLIC Aether::Engine imgui)
