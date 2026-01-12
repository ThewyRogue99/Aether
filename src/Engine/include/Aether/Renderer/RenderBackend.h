//
// Created by ThewyRogue99 on 11.01.2026.
//

#pragma once

#include <Aether/Renderer/Buffer.h>
#include <Aether/Renderer/RenderAPI.h>

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

        virtual BufferHandle CreateBuffer(const BufferDesc& desc, const void* initialData) = 0;
        virtual void DestroyBuffer(const BufferHandle& handle) = 0;
        virtual void UpdateBuffer(const BufferHandle& handle, uint32_t offset, const void* data, uint32_t size) = 0;
    };
}
