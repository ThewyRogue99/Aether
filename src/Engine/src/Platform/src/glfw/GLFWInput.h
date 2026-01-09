//
// Created by ThewyRogue99 on 8.01.2026.
//

#pragma once

#include <Aether/Input/KeyCode.h>
#include <Aether/Input/MouseButton.h>

namespace Aether::Platform {
    Engine::KeyCode TranslateGLFWKey(int glfwKey);

    Engine::MouseButton TranslateGLFWMouseButton(int glfwButton);
}
