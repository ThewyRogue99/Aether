//
// Created by ThewyRogue99 on 11.01.2026.
//

#include "GLFWGraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Aether::Platform {
    GLFWGraphicsContext::GLFWGraphicsContext(GLFWwindow* window, GraphicsAPI API) : m_window(window), m_API(API) { }

    GraphicsAPI GLFWGraphicsContext::GetAPI() const {
        return m_API;
    }

    void GLFWGraphicsContext::MakeCurrent() {
        glfwMakeContextCurrent(m_window);
    }

    void GLFWGraphicsContext::SwapBuffers() {
        glfwSwapBuffers(m_window);
    }

    void* GLFWGraphicsContext::GetProcAddress(const char* name) {
        return reinterpret_cast<void*>(glfwGetProcAddress(name));
    }

    GLFWwindow* GLFWGraphicsContext::GetWindowHandle() const {
        return m_window;
    }
}
