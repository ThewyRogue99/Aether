//
// Created by ThewyRogue99 on 22.01.2026.
//

#pragma once

#include <cstdint>

namespace Aether::Renderer {
    enum class TextureFormat : uint8_t {
        RGBA8,
        RGB8
    };

    struct TextureDesc {
        uint32_t width = 0;
        uint32_t height = 0;
        TextureFormat format = TextureFormat::RGBA8;
        const void* data = nullptr;
        const char* debugName = nullptr;
    };

    struct TextureHandle {
        uint32_t id = 0;
        explicit operator bool() const { return id != 0; }
    };
}
