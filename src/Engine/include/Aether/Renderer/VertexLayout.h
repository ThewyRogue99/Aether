//
// Created by ThewyRogue99 on 13.01.2026.
//

#pragma once

#include <cstdint>

namespace Aether::Renderer {
    enum class VertexFormat : uint8_t {
        Float,
        Float2,
        Float3,
        Float4,
    };

    inline uint32_t VertexFormatSize(const VertexFormat fmt) {
        switch (fmt) {
            case VertexFormat::Float:  return 4;
            case VertexFormat::Float2: return 8;
            case VertexFormat::Float3: return 12;
            case VertexFormat::Float4: return 16;
        }
        return 0;
    }

    struct VertexAttribute {
        uint32_t location = 0;
        VertexFormat format{};
        uint32_t offset = 0;
        bool normalized = false;
    };

    struct VertexLayout {
        const VertexAttribute* attributes = nullptr;
        uint32_t attributeCount = 0;
        uint32_t stride = 0;
    };
}
