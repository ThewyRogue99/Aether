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

        void SetViewport(int width, int height) override;

        BufferHandle CreateBuffer(const BufferDesc& desc, const void* initialData) override;
        void DestroyBuffer(const BufferHandle& handle) override;
        void UpdateBuffer(const BufferHandle& handle, uint32_t offset, const void* data, uint32_t size) override;

        ShaderHandle CreateShader(const ShaderDesc& desc) override;
        void DestroyShader(const ShaderHandle& handle) override;

        [[nodiscard]] uint32_t GetShaderProgram(const ShaderHandle& handle) const;

        PipelineHandle CreatePipeline(const PipelineDesc& desc) override;
        void DestroyPipeline(const PipelineHandle& handle) override;

        UniformBufferHandle CreateUniformBuffer(const UniformBufferDesc &desc) override;
        void DestroyUniformBuffer(const UniformBufferHandle& handle) override;
        void UpdateUniformBuffer(
            const UniformBufferHandle& handle,
            const void* data,
            uint32_t size,
            uint32_t offset
        ) override;

        void BindPipeline(const PipelineHandle &handle) override;
        void BindVertexBuffer(const BufferHandle &handle) override;
        void BindIndexBuffer(const BufferHandle &handle) override;
        void BindUniformBuffer(const UniformBufferHandle& handle, uint32_t bindingSlot) override;

        void Draw(uint32_t vertexCount, uint32_t firstVertex) override;
        void DrawIndexed(uint32_t indexCount, uint32_t firstIndex) override;

    private:
        class Impl;
        Engine::Scope<Impl> m_Impl;
    };
}
