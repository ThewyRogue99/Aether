//
// Created by ThewyRogue99 on 11.01.2026.
//

#pragma once

#include <Aether/Renderer/RenderAPI.h>

namespace Aether::Platform {
    class Window;
}

namespace Aether::Renderer {
    struct RendererInitInfo {
        Platform::Window* window = nullptr;
    };

    class Renderer {
    public:
        static void Init(const RendererInitInfo& info);
        static void Shutdown();

        static void BeginFrame();
        static void EndFrame();

        static void SetClearColor(float r, float g, float b, float a);
        static void Clear();

        static RenderAPI GetAPI();
    };
}
