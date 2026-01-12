//
// Created by ThewyRogue99 on 11.01.2026.
//

#pragma once

#include <Aether/Platform/GraphicsContext.h>

extern "C" {
    typedef struct GLFWwindow GLFWwindow;
}

namespace Aether::Platform {
    class GLFWGraphicsContext final : public GraphicsContext {
    public:
        explicit GLFWGraphicsContext(GLFWwindow* window, GraphicsAPI API = GraphicsAPI::OpenGL);

        [[nodiscard]] GraphicsAPI GetAPI() const override;

        void MakeCurrent() override;

        void SwapBuffers() override;

        void* GetProcAddress(const char* name) override;

        [[nodiscard]] GLFWwindow* GetWindowHandle() const;

    private:
        GLFWwindow* m_window = nullptr;
        GraphicsAPI m_API;
    };
}