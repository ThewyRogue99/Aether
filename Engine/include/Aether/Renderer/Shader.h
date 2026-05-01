//
// Created by ThewyRogue99 on 12.01.2026.
//

#pragma once

#include <cstdint>

namespace Aether::Renderer {
    struct ShaderHandle {
        uint32_t id = 0;
        explicit operator bool() const { return id != 0; }
    };

    enum class ShaderStage : uint8_t {
        Vertex = 1 << 0,
        Fragment = 1 << 1,
        Compute = 1 << 2
    };

    struct ShaderDesc {
        const char* debugName = nullptr;
        const char* vertexSource = nullptr;
        const char* fragmentSource = nullptr;
    };
}
