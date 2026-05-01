//
// Created by ThewyRogue99 on 21.01.2026.
//

#pragma once

#include <Aether/Renderer/Buffer.h>

namespace Aether::Renderer {
    struct Mesh {
        BufferHandle VertexBuffer;
        BufferHandle IndexBuffer;
        uint32_t VertexCount = 0;
        uint32_t IndexCount  = 0;

        bool IsIndexed() const { return IndexCount > 0; }
    };
}
