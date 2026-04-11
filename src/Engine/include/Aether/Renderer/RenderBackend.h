//
// Created by ThewyRogue99 on 11.01.2026.
//

#pragma once

#include <Aether/Renderer/Buffer.h>
#include <Aether/Renderer/Shader.h>
#include <Aether/Renderer/Pipeline.h>
#include <Aether/Renderer/RenderAPI.h>
#include <Aether/Renderer/Texture.h>
#include <Aether/Renderer/Sampler.h>
#include <Aether/Renderer/RenderSurface.h>
#include <Aether/Renderer/CommandBuffer.h>

namespace Aether::Platform {
    class GraphicsContext;
}

namespace Aether::Renderer {
    struct BackendInitInfo {
        Platform::GraphicsContext* context = nullptr;
    };

    class RenderBackend {
    public:
        virtual ~RenderBackend() = default;

        [[nodiscard]] virtual RenderAPI API() const = 0;

        virtual void Init(const BackendInitInfo& info) = 0;
        virtual void Shutdown() = 0;

        virtual void BeginFrame(const RenderSurfaceHandle& surface) = 0;
        virtual void EndFrame() = 0;

        virtual void SetClearColor(float r, float g, float b, float a) = 0;
        virtual void Clear() = 0;

        virtual BufferHandle CreateBuffer(const BufferDesc& desc, const void* initialData) = 0;
        virtual void DestroyBuffer(const BufferHandle& handle) = 0;
        virtual void UpdateBuffer(const BufferHandle& handle, uint32_t offset, const void* data, uint32_t size) = 0;

        virtual ShaderHandle CreateShader(const ShaderDesc& desc) = 0;
        virtual void DestroyShader(const ShaderHandle& handle) = 0;

        virtual PipelineHandle CreatePipeline(const PipelineDesc& desc) = 0;
        virtual void DestroyPipeline(const PipelineHandle& handle) = 0;

        virtual void BindPipeline(const PipelineHandle& handle) = 0;
        virtual void BindVertexBuffer(const BufferHandle& handle) = 0;
        virtual void BindIndexBuffer(const BufferHandle& handle) = 0;

        virtual UniformBufferHandle CreateUniformBuffer(const UniformBufferDesc& desc) = 0;
        virtual void DestroyUniformBuffer(const UniformBufferHandle& handle) = 0;
        virtual void UpdateUniformBuffer(
            const UniformBufferHandle& handle,
            const void* data,
            uint32_t size,
            uint32_t offset
        ) = 0;

        virtual void BindUniformBuffer(
            const UniformBufferHandle& handle,
            uint32_t bindingSlot
        ) = 0;

        virtual TextureHandle CreateTexture2D(const TextureDesc& desc) = 0;
        virtual void DestroyTexture(const TextureHandle& texture) = 0;

        virtual SamplerHandle CreateSampler(const SamplerDesc& desc) = 0;
        virtual void DestroySampler(const SamplerHandle& sampler) = 0;

        virtual void BindTexture2D(const TextureHandle& texture, uint32_t slot) = 0;
        virtual void BindSampler(const SamplerHandle& sampler, uint32_t slot) = 0;

        virtual RenderSurfaceHandle CreateRenderSurface(const RenderSurfaceDesc& desc) = 0;
        virtual void DestroyRenderSurface(const RenderSurfaceHandle& handle) = 0;
        virtual void ResizeRenderSurface(const RenderSurfaceHandle& handle, uint32_t width, uint32_t height) = 0;

        [[nodiscard]] virtual TextureHandle GetRenderSurfaceColorAttachment(const RenderSurfaceHandle& handle) const = 0;
        [[nodiscard]] virtual RenderSurfaceHandle GetPresentableSurface() const = 0;

        virtual void Draw(uint32_t vertexCount, uint32_t firstVertex) = 0;
        virtual void DrawIndexed(uint32_t indexCount, uint32_t firstIndex) = 0;

        virtual void Execute(const CommandBuffer& commandBuffer) = 0;
    };
}
