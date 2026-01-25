//
// Created by ThewyRogue99 on 13.01.2026.
//

#pragma once

#include <vector>

#include <Aether/Renderer/Shader.h>
#include <Aether/Renderer/VertexLayout.h>

#include <Aether/Core/String.h>

namespace Aether::Renderer {
    struct PipelineUniformBufferSlots {
        uint32_t camera = 0;
        uint32_t object = 0;
        uint32_t material = 0;
    };

    struct PipelineHandle {
        uint32_t id = 0;
        explicit operator bool() const { return id != 0; }

        PipelineUniformBufferSlots uniformBufferSlots{};
    };

    enum class CullMode : uint8_t { None, Back, Front };
    enum class DepthTest : uint8_t { Disabled, LessEqual };

    enum class UniformScope {
        Global, // camera, time
        Scene, // lights, fog
        Object, // model matrix
        Material // material params
    };

    struct UniformBufferLayoutDesc {
        Engine::String Name;
        uint32_t Binding;
        ShaderStage Stages;
        UniformScope Scope;
    };

    struct PipelineLayoutDesc {
        VertexLayout vertexLayout;
        std::vector<UniformBufferLayoutDesc> uniformBufferLayout;
    };

    struct PipelineDesc {
        ShaderHandle shader;
        PipelineLayoutDesc layout;

        CullMode cull = CullMode::Back;
        DepthTest depth = DepthTest::Disabled;
        bool blending = false;

        const char* debugName = nullptr;
    };
}
