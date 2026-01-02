add_library(AetherEngine SHARED Engine.cpp)

add_library(AetherEngineBuild INTERFACE)

target_compile_options(AetherEngineBuild INTERFACE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-fno-rtti>
    $<$<CXX_COMPILER_ID:MSVC>:/GR->
)

add_subdirectory(Core)

target_include_directories(AetherEngine PUBLIC
    ${ENGINE_INCLUDE_DIR}
    ${ENGINE_SOURCE_DIR}
)

target_link_libraries(AetherEngine PRIVATE
    AetherEngineBuild
    AetherEngineCore
)

add_library(Aether::Engine ALIAS AetherEngine)
