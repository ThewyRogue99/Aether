//
// Created by ThewyRogue99 on 28.12.2025.
//

#pragma once

#include <memory>

#include <Aether/Platform/GraphicsContext.h>

namespace Aether::Engine {
    class EventQueue;
}

namespace Aether::Platform {
    struct WindowProps {
        const char* Title;
        unsigned int Width;
        unsigned int Height;
        GraphicsAPI API;

        explicit WindowProps(
            const char* Title = "Aether",
            const unsigned int Width = 1280,
            const unsigned int Height = 720,
            const GraphicsAPI API = GraphicsAPI::OpenGL
        ) : Title(Title), Width(Width), Height(Height), API(API) { }
    };

    class Window {
    public:
        virtual ~Window();

        virtual void OnUpdate() = 0;

        [[nodiscard]] virtual unsigned int GetWidth() const = 0;
        [[nodiscard]] virtual unsigned int GetHeight() const = 0;

        [[nodiscard]] virtual const char* GetTitle() const = 0;
        virtual void SetTitle(const char* title) = 0;

        virtual void SetEventQueue(Engine::EventQueue* queue) = 0;

        [[nodiscard]] virtual bool ShouldClose() const = 0;

        [[nodiscard]] virtual GraphicsContext* GetGraphicsContext() const = 0;

        static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
    };
}
