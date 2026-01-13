//
// Created by ThewyRogue99 on 11.01.2026.
//

#pragma once

#include <cstdint>

#include <Aether/Renderer/Buffer.h>
#include <Aether/Renderer/Shader.h>
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

        static uint64_t GetFrameIndex();

        static BufferHandle CreateBuffer(const BufferDesc& desc, const void* initialData = nullptr);
        static void DestroyBuffer(const BufferHandle& handle);
        static void UpdateBuffer(const BufferHandle& handle, uint32_t offset, const void* data, uint32_t size);

        static ShaderHandle CreateShader(const ShaderDesc& desc);
        static void DestroyShader(const ShaderHandle& handle);
    };
}
