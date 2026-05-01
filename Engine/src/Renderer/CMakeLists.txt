add_library(aether_engine_renderer STATIC RenderThread.cpp Renderer.cpp CommandBuffer.cpp)

target_include_directories(aether_engine_renderer PUBLIC ${ENGINE_INCLUDE_DIR} ${ENGINE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(OpenGL)

target_link_libraries(aether_engine_renderer PRIVATE aether_engine_build Aether::Engine::Math glfw glad glm::glm)

add_library(Aether::Engine::Renderer ALIAS aether_engine_renderer)
