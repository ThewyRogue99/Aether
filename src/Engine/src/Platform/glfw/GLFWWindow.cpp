//
// Created by ThewyRogue99 on 26.12.2025.
//

#include "GLFWWindow.h"

#include <GLFW/glfw3.h>

#include <Aether/Events/KeyEvent.h>
#include <Aether/Events/EventQueue.h>
#include <Aether/Events/MouseEvent.h>

#include <glfw/GLFWInput.h>

#include "GLFWGraphicsContext.h"
#include "Aether/Events/WindowEvent.h"

namespace Aether::Platform {
    GLFWWindow::GLFWWindow(const WindowProps& props) {
        m_Title = props.Title;
        m_Width = props.Width;
        m_Height = props.Height;
        m_API = props.API;

        switch (m_API) {
            case GraphicsAPI::OpenGL: {
                glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

#if defined(AETHER_PLATFORM_MACOS)
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#else
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
#endif
            } break;
            default: {
                glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            } break;
        }

        const auto window = glfwCreateWindow(
            static_cast<int>(props.Width),
            static_cast<int>(props.Height),
            props.Title,
            nullptr,
            nullptr
        );

        m_Context = std::make_unique<GLFWGraphicsContext>(window, m_API);

        glfwSetWindowUserPointer(window, this);

        InitCallbacks();

        glfwMakeContextCurrent(nullptr);
    }

    GLFWWindow::~GLFWWindow() {
        glfwDestroyWindow(GetWindowHandle());
    }

    void GLFWWindow::OnUpdate() {
        if (GetWindowHandle()) {
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

        glfwSetWindowTitle(GetWindowHandle(), title);
    }

    void GLFWWindow::SetEventQueue(Engine::EventQueue* queue) {
        m_EventQueue = queue;
    }

    bool GLFWWindow::ShouldClose() const {
        return glfwWindowShouldClose(GetWindowHandle());
    }

    GraphicsContext* GLFWWindow::GetGraphicsContext() const {
        return m_Context.get();
    }

    void GLFWWindow::InitCallbacks() {
        glfwSetKeyCallback(GetWindowHandle(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

            self->OnKey(key, action);
        });

        glfwSetCursorPosCallback(GetWindowHandle(), [](GLFWwindow* window, double x, double y) {
            auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

            self->OnMouseMove(static_cast<float>(x), static_cast<float>(y));
        });

        glfwSetMouseButtonCallback(GetWindowHandle(), [](GLFWwindow* window, int button, int action, int mods) {
            auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

            self->OnMouseButton(button, action);
        });

        glfwSetWindowSizeCallback(GetWindowHandle(), [](GLFWwindow* window, int width, int height) {
            auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

            self->OnWindowResize(width, height);
        });

        glfwSetFramebufferSizeCallback(GetWindowHandle(), [](GLFWwindow* window, int width, int height) {
            auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

            self->OnFramebufferResize(width, height);
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

    void GLFWWindow::OnWindowResize(int width, int height) {
        if (m_EventQueue) {
            m_EventQueue->Push(std::make_unique<Engine::WindowResizeEvent>(width, height));
        }
    }

    void GLFWWindow::OnFramebufferResize(int width, int height) {
        if (m_EventQueue) {
            m_EventQueue->Push(std::make_unique<Engine::FramebufferResizeEvent>(width, height));
        }
    }

    GLFWwindow* GLFWWindow::GetWindowHandle() const {
        return m_Context->GetWindowHandle();
    }
}
