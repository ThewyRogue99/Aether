//
// Created by ThewyRogue99 on 4.01.2026.
//

#include <Aether/Platform/Window.h>

#include "glfw/GLFWWindow.h"

namespace Aether::Platform {
    Window::~Window() = default;

    std::shared_ptr<Window> Window::Create(const WindowProps &props) {
        return std::make_shared<GLFWWindow>(props);
    }
}