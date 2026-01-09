//
// Created by ThewyRogue99 on 8.01.2026.
//

#include "GLFWInput.h"

#include <GLFW/glfw3.h>

namespace Aether::Platform {
    Engine::KeyCode TranslateGLFWKey(int glfwKey) {
        return static_cast<Engine::KeyCode>(glfwKey);
    }

    Engine::MouseButton TranslateGLFWMouseButton(int glfwButton) {
        switch (glfwButton) {
            case GLFW_MOUSE_BUTTON_LEFT:
                return Engine::MouseButton::Left;
            case GLFW_MOUSE_BUTTON_RIGHT:
                return Engine::MouseButton::Right;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                return Engine::MouseButton::Middle;
            case GLFW_MOUSE_BUTTON_4:
                return Engine::MouseButton::Button_4;
            case GLFW_MOUSE_BUTTON_5:
                return Engine::MouseButton::Button_5;
            case GLFW_MOUSE_BUTTON_6:
                return Engine::MouseButton::Button_6;
            case GLFW_MOUSE_BUTTON_7:
                return Engine::MouseButton::Button_7;
            case GLFW_MOUSE_BUTTON_8:
                return Engine::MouseButton::Button_8;
            default: {
                if (glfwButton >= 0 && glfwButton <= 7)
                    return static_cast<Engine::MouseButton>(glfwButton);

                return Engine::MouseButton::Button_1;
            }
        }
    }
}
