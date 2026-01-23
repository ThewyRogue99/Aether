//
// Created by ThewyRogue99 on 22.01.2026.
//

#pragma once

#include <cstdint>

namespace Aether::Renderer {
    enum class Filter : uint8_t { Nearest, Linear };
    enum class Wrap : uint8_t { Repeat, Clamp };

    struct SamplerDesc {
        Filter min = Filter::Linear;
        Filter mag = Filter::Linear;
        Wrap u = Wrap::Repeat;
        Wrap v = Wrap::Repeat;
    };

    struct SamplerHandle {
        uint32_t id = 0;
        explicit operator bool() const { return id != 0; }
    };
}