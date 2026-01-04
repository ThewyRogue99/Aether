add_library(aether_engine_platform STATIC Platform.cpp Window.cpp)

target_include_directories(aether_engine_platform PRIVATE ${PLATFORM_INCLUDE_DIR})

add_subdirectory(glfw)

target_link_libraries(aether_engine_platform PRIVATE aether_engine_build glfw)

add_library(Aether::Engine::Platform ALIAS aether_engine_platform)
