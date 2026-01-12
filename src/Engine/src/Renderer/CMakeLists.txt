add_library(aether_engine_renderer STATIC RenderThread.cpp Renderer.cpp)

target_include_directories(aether_engine_renderer PUBLIC ${ENGINE_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(OpenGL)

target_link_libraries(aether_engine_renderer PRIVATE aether_engine_build glfw glad)

add_library(Aether::Engine::Renderer ALIAS aether_engine_renderer)
