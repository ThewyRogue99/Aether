add_executable(Sandbox SandboxApp.cpp SandboxLayer.cpp)
target_link_libraries(Sandbox PUBLIC
    Aether::Engine
)
