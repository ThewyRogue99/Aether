//
// Created by ThewyRogue99 on 11.01.2026.
//

#pragma once

#include <Aether/Core/Memory/Pointer.h>
#include <Aether/Renderer/RenderBackend.h>

namespace Aether::Platform {
    class GraphicsContext;
}

namespace Aether::Renderer {
    class GLContext;

    class GLBackend : public RenderBackend {
    public:
        GLBackend();
        ~GLBackend() override;

        [[nodiscard]] RenderAPI API() const override;

        void Init(const BackendInitInfo& info) override;
        void Shutdown() override;

        void BeginFrame() override;
        void EndFrame() override;

        void SetClearColor(float r, float g, float b, float a) override;
        void Clear() override;

        void Present() override;

        BufferHandle CreateBuffer(const BufferDesc& desc, const void* initialData) override;
        void DestroyBuffer(const BufferHandle& handle) override;
        void UpdateBuffer(const BufferHandle& handle, uint32_t offset, const void* data, uint32_t size) override;

    private:
        class Impl;
        Engine::Scope<Impl> m_Impl;
    };
}
