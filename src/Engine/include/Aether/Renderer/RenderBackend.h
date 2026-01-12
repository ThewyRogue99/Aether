//
// Created by ThewyRogue99 on 11.01.2026.
//

#pragma once

#include "Aether/Renderer/RenderAPI.h"

namespace Aether::Platform {
    class GraphicsContext;
}

namespace Aether::Renderer {
    struct BackendInitInfo {
        Platform::GraphicsContext* context = nullptr;

        unsigned int width = 0;
        unsigned int height = 0;
    };

    class RenderBackend {
    public:
        virtual ~RenderBackend() = default;

        [[nodiscard]] virtual RenderAPI API() const = 0;

        virtual void Init(const BackendInitInfo& info) = 0;
        virtual void Shutdown() = 0;

        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

        virtual void SetClearColor(float r, float g, float b, float a) = 0;
        virtual void Clear() = 0;

        virtual void Present() = 0;
    };
}
