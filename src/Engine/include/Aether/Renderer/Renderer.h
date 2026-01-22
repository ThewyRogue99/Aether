//
// Created by ThewyRogue99 on 11.01.2026.
//

#pragma once

#include <Aether/Renderer/Buffer.h>
#include <Aether/Renderer/Shader.h>
#include <Aether/Renderer/Pipeline.h>
#include <Aether/Renderer/RenderAPI.h>
#include <Aether/Renderer/Mesh.h>
#include <Aether/Renderer/Material.h>

#include <Aether/Math/Matrix.h>

namespace Aether::Platform {
    class Window;
}

namespace Aether::Renderer {
    struct ObjectDesc {
        Math::Matrix4f Model;
    };

    struct CameraDesc {
        Math::Matrix4f ViewProjection;
    };

    struct RendererInitInfo {
        Platform::Window* window = nullptr;
    };

    class Renderer {
    public:
        static void Init(const RendererInitInfo& info);
        static void Shutdown();

        static void BeginFrame();
        static void EndFrame();

        static void SetCamera(const CameraDesc& camera);

        static void SetClearColor(float r, float g, float b, float a);
        static void Clear();

        static void SetViewport(int width, int height);

        static RenderAPI GetAPI();

        static uint64_t GetFrameIndex();

        static BufferHandle CreateBuffer(const BufferDesc& desc, const void* initialData = nullptr);
        static void DestroyBuffer(const BufferHandle& handle);
        static void UpdateBuffer(const BufferHandle& handle, uint32_t offset, const void* data, uint32_t size);

        static ShaderHandle CreateShader(const ShaderDesc& desc);
        static void DestroyShader(const ShaderHandle& handle);

        static PipelineHandle CreatePipeline(const PipelineDesc& desc);
        static void DestroyPipeline(const PipelineHandle& handle);

        static Material CreateMaterial(const PipelineHandle& pipeline, const char* debugName = nullptr);
        static void DestroyMaterial(Material& material);

        static void SetMaterialColor(Material& material, const Math::Vector4f& color);

        static void BindPipeline(const PipelineHandle& handle);
        static void BindVertexBuffer(const BufferHandle& handle);
        static void BindIndexBuffer(const BufferHandle& handle);

        static void DrawMesh(const Mesh& mesh, const Material& material, const Math::Matrix4f& model);
    };
}
