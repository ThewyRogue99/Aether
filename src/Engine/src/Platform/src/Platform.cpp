//
// Created by ThewyRogue99 on 4.01.2026.
//

#include <Aether/Platform/Platform.h>

#include <stdexcept>

#include <GLFW/glfw3.h>

namespace Aether::Platform {
    static bool s_GLFWInitialized = false;

    void Platform::Init() {
        if (!s_GLFWInitialized) {
            if (!glfwInit()) {
                throw std::runtime_error("Failed to initialize GLFW");
            }

            s_GLFWInitialized = true;
        }
    }

    double Platform::GetTime() { return glfwGetTime(); }

    void Platform::Shutdown() {
        if (s_GLFWInitialized) {
            glfwTerminate();
            s_GLFWInitialized = false;
        }
    }
}
