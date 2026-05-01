//
// Created by ThewyRogue99 on 12.01.2026.
//

#pragma once

#include <cstdint>

namespace Aether::Renderer {
    struct BufferHandle {
        uint32_t id = 0;
        explicit operator bool() const { return id != 0; }
    };

    enum class BufferUsage : uint8_t {
        Vertex,
        Index,
        Uniform
    };

    enum class BufferAccess : uint8_t {
        Static,
        Dynamic
    };

    struct BufferDesc {
        BufferUsage usage;
        BufferAccess access;
        uint32_t size;
    };

    struct UniformBufferHandle {
        uint32_t id = 0;
        explicit operator bool() const { return id != 0; }
    };

    enum class UniformUsage : uint8_t {
        PerFrame,
        PerObject
    };

    struct UniformBufferDesc {
        uint32_t size;
        UniformUsage usage;
        const char* debugName = nullptr;
    };
}
