//
// Created by ThewyRogue99 on 11.01.2026.
//

#include <Aether/Renderer/Renderer.h>

#include <Aether/Renderer/RenderThread.h>
#include <Aether/Renderer/RenderBackend.h>

#include <Aether/Core/Assert.h>
#include <Aether/Platform/Window.h>
#include <Aether/Core/Memory/Pointer.h>

#include <glm/mat4x4.hpp>

#include "Math/Interop.h"
#include "OpenGL/GLBackend.h"

namespace Aether::Renderer {
    struct CameraData {
        glm::mat4 ViewProjection;
    } s_CameraData;

    static UniformBufferHandle s_CameraUBO;
    static UniformBufferHandle s_ObjectUBO;
    static RenderThread s_RenderThread;
    static Engine::Scope<RenderBackend> s_Backend;
    static uint64_t s_FrameIndex = 0;

    static TextureHandle s_WhiteTexture;
    static SamplerHandle s_DefaultSampler;

    static Platform::Window* s_Window = nullptr;

    static BackendInitInfo MakeBackendInitInfo() {
        BackendInitInfo bi{};
        bi.context = s_Window ? s_Window->GetGraphicsContext() : nullptr;
        bi.width  = s_Window ? s_Window->GetWidth() : 0;
        bi.height = s_Window ? s_Window->GetHeight() : 0;
        return bi;
    }

    void Renderer::Init(const RendererInitInfo& info) {
        AETHER_ASSERT_MSG(info.window, "Window cannot be null");
        s_Window = info.window;

        const auto context = s_Window->GetGraphicsContext();

        UniformBufferDesc desc{};
        desc.size = sizeof(CameraData);
        desc.usage = UniformUsage::PerFrame;
        desc.debugName = "CameraUBO";

        UniformBufferDesc objDesc{};
        objDesc.size = sizeof(glm::mat4);
        objDesc.usage = UniformUsage::PerObject;
        objDesc.debugName = "ObjectUBO";

        s_RenderThread.Start();

        s_RenderThread.Enqueue([=](){
            switch (context->GetAPI()) {
                case Platform::GraphicsAPI::OpenGL: s_Backend = Engine::MakeScope<GLBackend>(); break;
                default: /* TODO */ break;
            }

            if (s_Backend) {
                s_Backend->Init(MakeBackendInitInfo());

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

                s_CameraUBO = s_Backend->CreateUniformBuffer(desc);
                s_ObjectUBO = s_Backend->CreateUniformBuffer(objDesc);
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
        s_Window = nullptr;
    }

    void Renderer::BeginFrame() {
        s_RenderThread.Enqueue([](){
            if (s_Backend) {
                s_Backend->BeginFrame();
            }
        });
    }

    void Renderer::EndFrame() {
        s_RenderThread.Enqueue([](){
            if (s_Backend) {
                s_Backend->EndFrame();
                s_Backend->Present();
            }
        });

        ++s_FrameIndex;
    }

    void Renderer::SetCamera(const CameraDesc& camera) {
        s_CameraData = { Math::ToGLM(camera.ViewProjection) };

        s_RenderThread.Enqueue([](){
            if (s_Backend) {
                s_Backend->UpdateUniformBuffer(
                    s_CameraUBO,
                    &s_CameraData,
                    sizeof(CameraData),
                    0
                );
            }
        });
    }

    void Renderer::SetClearColor(float r, float g, float b, float a) {
        s_RenderThread.Enqueue([=](){
            if (s_Backend) s_Backend->SetClearColor(r, g, b, a);
        });
    }

    void Renderer::Clear() {
        s_RenderThread.Enqueue([](){
            if (s_Backend) s_Backend->Clear();
        });
    }

    void Renderer::SetViewport(int width, int height) {
        s_RenderThread.Enqueue([=](){
            if (s_Backend) s_Backend->SetViewport(width, height);
        });
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

    void Renderer::DrawMesh(const Mesh& mesh, const Material& material, const Math::Matrix4f& model) {
        glm::mat4 modelGLM = Math::ToGLM(model);

        s_RenderThread.Enqueue([=]() {
            if (!s_Backend) return;

            s_Backend->BindPipeline(material.Pipeline);

            const auto& uniformBufferSlots = material.Pipeline.uniformBufferSlots;

            s_Backend->BindUniformBuffer(s_CameraUBO, uniformBufferSlots.camera);

            s_Backend->UpdateUniformBuffer(s_ObjectUBO, &modelGLM, sizeof(glm::mat4), 0);
            s_Backend->BindUniformBuffer(s_ObjectUBO, uniformBufferSlots.object);
            s_Backend->BindUniformBuffer(material.UBO, uniformBufferSlots.material);

            s_Backend->BindVertexBuffer(mesh.VertexBuffer);

            s_Backend->BindTexture2D(material.Albedo, 0);
            s_Backend->BindSampler(material.Sampler, 0);

            if (mesh.IsIndexed()) {
                s_Backend->BindIndexBuffer(mesh.IndexBuffer);
                s_Backend->DrawIndexed(mesh.IndexCount, 0);
            } else {
                s_Backend->Draw(mesh.VertexCount, 0);
            }
        });
    }
}
