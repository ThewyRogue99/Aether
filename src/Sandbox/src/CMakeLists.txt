add_executable(Sandbox main.cpp SandboxApp.cpp)
target_link_libraries(Sandbox PUBLIC
    Aether::Engine
)
