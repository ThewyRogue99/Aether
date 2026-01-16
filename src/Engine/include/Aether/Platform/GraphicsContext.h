//
// Created by ThewyRogue99 on 11.01.2026.
//

#pragma once

namespace Aether::Platform {
    // TODO: Temporary type, change in the future
    struct FramebufferSize {
        int Width = 0;
        int Height = 0;
    };

    enum class GraphicsAPI {
        None = 0,
        OpenGL,
        Vulkan
    };

    class GraphicsContext {
    public:
        virtual ~GraphicsContext() = default;

        [[nodiscard]] virtual GraphicsAPI GetAPI() const = 0;

        virtual void MakeCurrent() = 0;
        virtual void SwapBuffers() = 0;

        [[nodiscard]] virtual FramebufferSize GetFrameBufferSize() const = 0;

        virtual void* GetProcAddress(const char* name) = 0;
    };
}
