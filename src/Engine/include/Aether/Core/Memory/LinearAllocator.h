//
// Created by ThewyRogue99 on 1.01.2026.
//

#pragma once

#include <cstddef>
#include <cstdint>

#include "Aether/Core/Core.h"

namespace Aether::Core {
    class AETHER_API LinearAllocator {
    public:
        explicit LinearAllocator(uint32_t blockSize = 256 * 1024);
        ~LinearAllocator();

        LinearAllocator(const LinearAllocator&) = delete;
        LinearAllocator& operator=(const LinearAllocator&) = delete;

        void* allocate(uint32_t size, uint32_t alignment = alignof(std::max_align_t));

        void reset(bool freeMemory = false);

    private:
        struct Block {
            Block*   next;
            uint32_t used;
            uint32_t capacity;
            char     data[1];
        };

        uint32_t m_blockSize;
        Block* m_head = nullptr;

        [[nodiscard]] bool fits(uint32_t size, uint32_t align) const;

        void addBlock(uint32_t minSize);

    public:
        struct Marker {
            Block* block;
            uint32_t used;
        };

        [[nodiscard]] Marker mark() const;

        void rewind(Marker m);
    };
}
