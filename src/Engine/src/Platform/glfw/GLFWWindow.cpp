//
// Created by ThewyRogue99 on 26.12.2025.
//

#include "GLFWWindow.h"

#include <GLFW/glfw3.h>

#include <Aether/Events/KeyEvent.h>
#include <Aether/Events/EventQueue.h>
#include <Aether/Events/MouseEvent.h>

#include <glfw/GLFWInput.h>

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

        glfwSetWindowUserPointer(m_Window, this);

        InitCallbacks();
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

    void GLFWWindow::SetEventQueue(Engine::EventQueue* queue) {
        m_EventQueue = queue;
    }

    bool GLFWWindow::ShouldClose() const {
        return glfwWindowShouldClose(m_Window);
    }

    void* GLFWWindow::GetNativeWindow() const {
        return m_Window;
    }

    void GLFWWindow::InitCallbacks() {
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

            self->OnKey(key, action);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) {
            auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

            self->OnMouseMove(static_cast<float>(x), static_cast<float>(y));
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
            auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

            self->OnMouseButton(button, action);
        });
    }

    void GLFWWindow::OnKey(int key, int action) {
        if (m_EventQueue) {
            Engine::KeyCode code = TranslateGLFWKey(key);

            if (action == GLFW_PRESS) {
                m_EventQueue->Push(std::make_unique<Engine::KeyPressedEvent>(code, false));
            } else if (action == GLFW_RELEASE) {
                m_EventQueue->Push(std::make_unique<Engine::KeyReleasedEvent>(code));
            }
        }
    }

    void GLFWWindow::OnMouseMove(float x, float y) {
        if (m_EventQueue) {
            m_EventQueue->Push(std::make_unique<Engine::MouseMovedEvent>(x, y));
        }
    }

    void GLFWWindow::OnMouseButton(int button, int action) {
        if (m_EventQueue) {
            Engine::MouseButton mb = TranslateGLFWMouseButton(button);

            if (action == GLFW_PRESS) {
                m_EventQueue->Push(std::make_unique<Engine::MouseButtonPressedEvent>(mb));
            } else if (action == GLFW_RELEASE) {
                m_EventQueue->Push(std::make_unique<Engine::MouseButtonReleasedEvent>(mb));
            }
        }
    }
}
