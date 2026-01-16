//
// Created by ThewyRogue99 on 13.01.2026.
//

#pragma once

#include <Aether/Renderer/Shader.h>
#include <Aether/Renderer/VertexLayout.h>

namespace Aether::Renderer {
    struct PipelineHandle {
        uint32_t id = 0;
        explicit operator bool() const { return id != 0; }
    };

    enum class CullMode : uint8_t { None, Back, Front };
    enum class DepthTest : uint8_t { Disabled, LessEqual };

    struct PipelineDesc {
        ShaderHandle shader;
        VertexLayout layout;

        CullMode cull = CullMode::Back;
        DepthTest depth = DepthTest::Disabled;
        bool blending = false;

        const char* debugName = nullptr;
    };
}
