//
// Created by ThewyRogue99 on 26.12.2025.
//

#pragma once

#include <Aether/Platform/Window.h>

#include <string>

extern "C" {
    typedef struct GLFWwindow GLFWwindow;
}

namespace Aether::Platform {
    class GLFWWindow : public Window {
    public:
        explicit GLFWWindow(const WindowProps& props);
        ~GLFWWindow() override;

        void OnUpdate() override;

        [[nodiscard]] unsigned GetWidth() const override;
        [[nodiscard]] unsigned GetHeight() const override;

        [[nodiscard]] const char* GetTitle() const override;
        void SetTitle(const char* title) override;

        [[nodiscard]] bool ShouldClose() const override;

        [[nodiscard]] void* GetNativeWindow() const override;

    private:
        GLFWwindow* m_Window = nullptr;

        std::string m_Title;
        unsigned int m_Width, m_Height;
    };
}
