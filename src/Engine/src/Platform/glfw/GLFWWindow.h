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
    class GLFWGraphicsContext;

    class GLFWWindow : public Window {
    public:
        explicit GLFWWindow(const WindowProps& props);
        ~GLFWWindow() override;

        void OnUpdate() override;

        [[nodiscard]] unsigned GetWidth() const override;
        [[nodiscard]] unsigned GetHeight() const override;

        [[nodiscard]] const char* GetTitle() const override;
        void SetTitle(const char* title) override;

        void SetEventQueue(Engine::EventQueue* queue) override;

        [[nodiscard]] bool ShouldClose() const override;

        [[nodiscard]] GraphicsContext* GetGraphicsContext() const override;
        [[nodiscard]] void* GetNativeHandle() const override;

    private:
        void InitCallbacks();

        void OnKey(int key, int action);
        void OnMouseMove(float x, float y);
        void OnMouseButton(int button, int action);
        void OnWindowResize(int width, int height);
        void OnFramebufferResize(int width, int height);

        [[nodiscard]] GLFWwindow* GetWindowHandle() const;

    private:
        std::unique_ptr<GLFWGraphicsContext> m_Context;

        std::string m_Title;
        unsigned int m_Width, m_Height;
        GraphicsAPI m_API;

        Engine::EventQueue* m_EventQueue = nullptr;
    };
}
