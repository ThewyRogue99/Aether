//
// Created by ThewyRogue99 on 15.02.2026.
//

#pragma once

#include <cstdint>

namespace Aether::ECS {
    struct Entity {
        uint64_t Value = 0;

        bool operator==(const Entity& other) const = default;
        bool operator!=(const Entity& other) const = default;
    };

    constexpr Entity INVALID_ENTITY{ 0 };

    inline uint32_t EntityIndex(const Entity& e) {
        return static_cast<uint32_t>(e.Value & 0xFFFFFFFFull);
    }

    inline uint32_t EntityGeneration(const Entity& e) {
        return static_cast<uint32_t>(e.Value >> 32);
    }

    inline Entity MakeEntity(uint32_t index, uint32_t generation) {
        return Entity{
            (static_cast<uint64_t>(generation) << 32) |
            static_cast<uint64_t>(index)
        };
    }
}