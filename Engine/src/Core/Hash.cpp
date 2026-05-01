//
// Created by ThewyRogue99 on 1.01.2026.
//

#include <Aether/Core/Hash.h>

#include <Aether/Core/StringView.h>

namespace Aether::Engine {
    uint32_t FNV1a32(const StringView& sv) {
        uint32_t h = 2166136261u;
        for (uint32_t i = 0; i < sv.size(); ++i) {
            h ^= static_cast<uint8_t>(sv.data()[i]);
            h *= 16777619u;
        }
        return h;
    }
}
