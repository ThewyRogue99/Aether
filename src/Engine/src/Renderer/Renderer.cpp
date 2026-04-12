//
// Created by ThewyRogue99 on 11.01.2026.
//

#include <Aether/Renderer/Renderer.h>

#include <Aether/Renderer/RenderThread.h>
#include <Aether/Renderer/RenderBackend.h>
#include <Aether/Renderer/CommandBuffer.h>

#include <Aether/Core/Assert.h>
#include <Aether/Platform/GraphicsContext.h>
#include <Aether/Core/Memory/Pointer.h>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "Math/Interop.h"
#include "OpenGL/GLBackend.h"

namespace Aether::Renderer {
    static RenderThread s_RenderThread;
    static Engine::Scope<RenderBackend> s_Backend;
    static uint64_t s_FrameIndex = 0;

    static TextureHandle s_WhiteTexture;
    static SamplerHandle s_DefaultSampler;

    static Platform::GraphicsContext* s_Context = nullptr;

    static CommandBuffer s_CommandBuffer;

    void Renderer::Init(const RendererInitInfo& info) {
        AETHER_ASSERT_MSG(info.context, "GraphicsContext cannot be null");
        s_Context = info.context;

        s_RenderThread.Start();

        s_RenderThread.Enqueue([=](){
            switch (s_Context->GetAPI()) {
                case Platform::GraphicsAPI::OpenGL: s_Backend = Engine::MakeScope<GLBackend>(); break;
                default: /* TODO */ break;
            }

            if (s_Backend) {
                BackendInitInfo bi{};
                bi.context = s_Context;
                s_Backend->Init(bi);

                uint8_t white[4] = { 255, 255, 255, 255 };

                TextureDesc td{};
                td.width  = 1;
                td.height = 1;
                td.format = TextureFormat::RGBA8;
                td.data   = white;

                s_WhiteTexture = s_Backend->CreateTexture2D(td);

                SamplerDesc sd{};
                sd.min = Filter::Linear;
                sd.mag = Filter::Linear;

                s_DefaultSampler = s_Backend->CreateSampler(sd);
            }
        });

        s_RenderThread.Flush();
    }

    void Renderer::Shutdown() {
        s_RenderThread.Enqueue([](){
            if (s_Backend) {
                s_Backend->Shutdown();
                s_Backend.reset();
            }
        });

        s_RenderThread.Flush();

        s_RenderThread.Stop();
        s_Context = nullptr;
    }

    void Renderer::BeginFrame(const RenderSurfaceHandle& surface) {
        s_CommandBuffer.Begin();
        s_CommandBuffer.BeginFrame(surface);
    }

    void Renderer::EndFrame() {
        s_CommandBuffer.EndFrame();
        s_CommandBuffer.End();

        CommandBuffer recorded = std::move(s_CommandBuffer);
        s_RenderThread.Enqueue([buf = std::move(recorded)]() {
            if (s_Backend) s_Backend->Execute(buf);
        });

        ++s_FrameIndex;
    }

    void Renderer::SetCamera(const CameraDesc& camera) {
        s_CommandBuffer.SetCamera(camera.ViewProjection);
    }

    void Renderer::SetClearColor(float r, float g, float b, float a) {
        s_CommandBuffer.SetClearColor(r, g, b, a);
    }

    void Renderer::Clear() {
        s_CommandBuffer.Clear();
    }

    void Renderer::DrawMesh(const Mesh& mesh, const Material& material, const Math::Matrix4f& model) {
        s_CommandBuffer.DrawMesh(mesh, material, model);
    }

    uint32_t Renderer::GetNativeTextureID(const TextureHandle& handle) {
        return s_Backend->GetNativeTextureID(handle);
    }

    RenderAPI Renderer::GetAPI() {
        return s_Backend ? s_Backend->API() : RenderAPI::None;
    }

    uint64_t Renderer::GetFrameIndex() {
        return s_FrameIndex;
    }

    BufferHandle Renderer::CreateBuffer(const BufferDesc& desc, const void* initialData) {
        BufferHandle handle;

        s_RenderThread.Enqueue([&]() {
            handle = s_Backend->CreateBuffer(desc, initialData);
        });

        s_RenderThread.Flush();
        return handle;
    }

    void Renderer::DestroyBuffer(const BufferHandle& handle) {
        s_RenderThread.Enqueue([=]() {
            s_Backend->DestroyBuffer(handle);
        });
    }

    void Renderer::UpdateBuffer(const BufferHandle& handle, uint32_t offset, const void *data, uint32_t size) {
        s_RenderThread.Enqueue([=]() {
            s_Backend->UpdateBuffer(handle, offset, data, size);
        });
    }

    ShaderHandle Renderer::CreateShader(const ShaderDesc& desc) {
        ShaderHandle out;

        s_RenderThread.Enqueue([&] {
            out = s_Backend->CreateShader(desc);
        });

        s_RenderThread.Flush();
        return out;
    }

    void Renderer::DestroyShader(const ShaderHandle& handle) {
        s_RenderThread.Enqueue([=] {
            s_Backend->DestroyShader(handle);
        });
    }

    PipelineHandle Renderer::CreatePipeline(const PipelineDesc& desc) {
        PipelineHandle handle;

        s_RenderThread.Enqueue([&]() {
            handle = s_Backend->CreatePipeline(desc);
        });

        s_RenderThread.Flush();

        PipelineUniformBufferSlots slots{};

        for (const auto& layout : desc.layout.uniformBufferLayout) {
            if (layout.Name == "Camera") {
                slots.camera = layout.Binding;
            } else if (layout.Name == "Object") {
                slots.object = layout.Binding;
            } else if (layout.Name == "Light") {
                slots.light = layout.Binding;
            } else if (layout.Name == "Material") {
                slots.material = layout.Binding;
            }
        }

        handle.uniformBufferSlots = slots;

        return handle;
    }

    void Renderer::DestroyPipeline(const PipelineHandle& handle) {
        s_RenderThread.Enqueue([=]() {
            s_Backend->DestroyPipeline(handle);
        });
    }

    Material Renderer::CreateMaterial(const PipelineHandle& pipeline, const char* debugName) {
        Material mat{};
        mat.Pipeline = pipeline;
        mat.Albedo = s_WhiteTexture;
        mat.Sampler = s_DefaultSampler;

        UniformBufferDesc desc{};
        desc.size = sizeof(glm::vec4);
        desc.usage = UniformUsage::PerObject;
        desc.debugName = debugName ? debugName : "MaterialUBO";

        s_RenderThread.Enqueue([&]() {
            constexpr glm::vec4 baseColor = { 1.0f, 1.0f, 1.0f, 1.0f };

            mat.UBO = s_Backend->CreateUniformBuffer(desc);
            s_Backend->UpdateUniformBuffer(mat.UBO, &baseColor, sizeof(glm::vec4), 0);
        });

        s_RenderThread.Flush();
        return mat;
    }

    void Renderer::DestroyMaterial(Material& material) {
        if (!material.UBO) return;

        s_RenderThread.Enqueue([=]() {
            s_Backend->DestroyUniformBuffer(material.UBO);
        });

        material.UBO = {};
    }

    TextureHandle Renderer::CreateTexture2D(const TextureDesc& desc) {
        TextureHandle out;

        s_RenderThread.Enqueue([&]() {
            out = s_Backend->CreateTexture2D(desc);
        });

        s_RenderThread.Flush();
        return out;
    }

    void Renderer::DestroyTexture(TextureHandle texture) {
        if (!texture) return;

        s_RenderThread.Enqueue([=]() {
            s_Backend->DestroyTexture(texture);
        });
    }

    SamplerHandle Renderer::CreateSampler(const SamplerDesc& desc) {
        SamplerHandle out;

        s_RenderThread.Enqueue([&]() {
            out = s_Backend->CreateSampler(desc);
        });

        s_RenderThread.Flush();
        return out;
    }

    void Renderer::DestroySampler(const SamplerHandle& sampler) {
        if (!sampler) return;

        s_RenderThread.Enqueue([=]() {
            s_Backend->DestroySampler(sampler);
        });
    }

    RenderSurfaceHandle Renderer::CreateRenderSurface(const RenderSurfaceDesc& desc) {
        RenderSurfaceHandle out;

        s_RenderThread.Enqueue([&]() {
            out = s_Backend->CreateRenderSurface(desc);
        });

        s_RenderThread.Flush();
        return out;
    }

    void Renderer::DestroyRenderSurface(const RenderSurfaceHandle& handle) {
        if (!handle) return;

        s_RenderThread.Enqueue([=]() {
            s_Backend->DestroyRenderSurface(handle);
        });
    }

    void Renderer::ResizeRenderSurface(const RenderSurfaceHandle& handle, uint32_t width, uint32_t height) {
        s_RenderThread.Enqueue([=]() {
            if (s_Backend) s_Backend->ResizeRenderSurface(handle, width, height);
        });

        s_RenderThread.Flush();
    }

    TextureHandle Renderer::GetRenderSurfaceColorAttachment(const RenderSurfaceHandle& handle) {
        TextureHandle out;

        s_RenderThread.Enqueue([&]() {
            out = s_Backend->GetRenderSurfaceColorAttachment(handle);
        });

        s_RenderThread.Flush();
        return out;
    }

    RenderSurfaceSize Renderer::GetRenderSurfaceSize(const RenderSurfaceHandle& handle) {
        RenderSurfaceSize out;

        s_RenderThread.Enqueue([&]() {
            out = s_Backend->GetRenderSurfaceSize(handle);
        });

        s_RenderThread.Flush();
        return out;
    }

    RenderSurfaceHandle Renderer::GetPresentableSurface() {
        RenderSurfaceHandle out;

        s_RenderThread.Enqueue([&]() {
            out = s_Backend->GetPresentableSurface();
        });

        s_RenderThread.Flush();
        return out;
    }

    void Renderer::SetMaterialColor(Material& material, const Math::Vector4f& color) {
        s_RenderThread.Enqueue([=]() {
            const glm::vec4 colorData = Math::ToGLM(color);

            s_Backend->UpdateUniformBuffer(material.UBO, &colorData, sizeof(glm::vec4), 0);
        });
    }

    void Renderer::BindPipeline(const PipelineHandle& handle) {
        s_RenderThread.Enqueue([=]() {
            s_Backend->BindPipeline(handle);
        });
    }

    void Renderer::BindVertexBuffer(const BufferHandle& handle) {
        s_RenderThread.Enqueue([=]() {
            s_Backend->BindVertexBuffer(handle);
        });
    }

    void Renderer::BindIndexBuffer(const BufferHandle& handle) {
        s_RenderThread.Enqueue([=]() {
            s_Backend->BindIndexBuffer(handle);
        });
    }
}
