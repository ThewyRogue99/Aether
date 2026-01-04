//
// Created by ThewyRogue99 on 26.12.2025.
//

#include "GLFWWindow.h"

#include <GLFW/glfw3.h>

namespace Aether::Platform {
    GLFWWindow::GLFWWindow(const WindowProps& props) {
        m_Title = props.Title;
        m_Width = props.Width;
        m_Height = props.Height;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_Window = glfwCreateWindow(
            static_cast<int>(props.Width),
            static_cast<int>(props.Height),
            props.Title,
            nullptr,
            nullptr
        );
    }

    GLFWWindow::~GLFWWindow() {
        glfwDestroyWindow(m_Window);
    }

    void GLFWWindow::OnUpdate() {
        if (m_Window) {
            glfwPollEvents();
        }
    }

    unsigned GLFWWindow::GetWidth() const {
        return m_Width;
    }

    unsigned GLFWWindow::GetHeight() const {
        return m_Height;
    }

    const char* GLFWWindow::GetTitle() const {
        return m_Title.c_str();
    }

    void GLFWWindow::SetTitle(const char* title) {
        m_Title = title;

        glfwSetWindowTitle(m_Window, title);
    }

    bool GLFWWindow::ShouldClose() const {
        return glfwWindowShouldClose(m_Window);
    }

    void* GLFWWindow::GetNativeWindow() const {
        return m_Window;
    }
}
