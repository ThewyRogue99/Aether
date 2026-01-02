//
// Created by ThewyRogue99 on 28.12.2025.
//

#pragma once

#include <Aether/Core/Core.h>
#include <Aether/Core/String.h>
#include <Aether/Core/Memory/Pointer.h>

namespace Aether::Core {
    struct WindowProps {
        const char* title;
        unsigned int width;
        unsigned int height;

        explicit WindowProps(
            const char* title = "Aether Engine",
            unsigned int width = 1280,
            unsigned int height = 720
        ) : title(title), width(width), height(height) { }
    };

    class Window
    {
    public:
        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        virtual void SetVSync(bool Enabled) = 0;
        virtual bool IsVsync() const = 0;

        virtual void SetTitle(const String& title) = 0;

        virtual void* GetNativeWindow() const = 0;

        AETHER_API static Ref<Window> Create(const WindowProps& props = WindowProps());
    };
}
