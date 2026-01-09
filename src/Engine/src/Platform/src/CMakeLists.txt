add_library(aether_engine_platform STATIC Platform.cpp Window.cpp)

target_include_directories(aether_engine_platform PUBLIC ${ENGINE_INCLUDE_DIR} ${PLATFORM_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(glfw)

target_link_libraries(aether_engine_platform PRIVATE aether_engine_build glfw)

add_library(Aether::Engine::Platform ALIAS aether_engine_platform)
