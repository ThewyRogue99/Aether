add_library(aether_engine_math STATIC
    Math.cpp
    Interop.cpp
)

target_include_directories(aether_engine_math PRIVATE ${ENGINE_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(aether_engine_math PRIVATE
    aether_engine_build
    glm::glm
)

add_library(Aether::Engine::Math ALIAS aether_engine_math)
