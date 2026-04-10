//
// Created by ThewyRogue99 on 11.04.2026.
//

#pragma once

#include <cstdint>

#include <Aether/Renderer/Texture.h>

namespace Aether::Renderer {
    struct RenderSurfaceHandle {
        uint32_t id = 0;
        explicit operator bool() const { return id != 0; }
    };

    struct RenderSurfaceDesc {
        uint32_t width = 0;
        uint32_t height = 0;
        TextureFormat colorFormat = TextureFormat::RGBA8;
        const char* debugName = nullptr;
    };
}
