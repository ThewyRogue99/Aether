//
// Created by ThewyRogue99 on 1.01.2026.
//

#include <Aether/Core/Memory/LinearAllocator.h>

#include <new>
#include <Aether/Core/Assert.h>

uintptr_t alignUp(uintptr_t p, uint32_t a) {
    return (p + a - 1) & ~(static_cast<uintptr_t>(a) - 1);
}

constexpr bool isPowerOfTwo(uint32_t x) {
    return x != 0 && (x & (x - 1)) == 0;
}

namespace Aether::Core {
    LinearAllocator::LinearAllocator(uint32_t blockSize) : m_blockSize(blockSize) {}

    LinearAllocator::~LinearAllocator() { reset(true); }

    void* LinearAllocator::allocate(uint32_t size, uint32_t alignment) {
        AETHER_ASSERT(size > 0);
        AETHER_ASSERT(isPowerOfTwo(alignment));

        if (!m_head || !fits(size, alignment)) {
            addBlock(size + alignment);
        }

        uintptr_t base = reinterpret_cast<uintptr_t>(m_head->data + m_head->used);
        uintptr_t aligned = alignUp(base, alignment);

        uint32_t padding = static_cast<uint32_t>(aligned - base);
        m_head->used += padding;

        void* result = m_head->data + m_head->used;
        m_head->used += size;

        return result;
    }

    void LinearAllocator::reset(bool freeMemory) {
        if (freeMemory) {
            while (m_head) {
                Block* next = m_head->next;
                std::free(m_head);
                m_head = next;
            }
        } else {
            for (Block* b = m_head; b; b = b->next) {
                b->used = 0;
            }
        }
    }

    bool LinearAllocator::fits(uint32_t size, uint32_t align) const {
        uint32_t remaining = m_head->capacity - m_head->used;
        return remaining >= size + align;
    }

    void LinearAllocator::addBlock(uint32_t minSize) {
        const uint32_t cap = (minSize > m_blockSize) ? minSize : m_blockSize;
        const size_t total = sizeof(Block) + cap;

        Block* b = static_cast<Block*>(std::malloc(total));
        AETHER_ASSERT(b);

        b->next = m_head;
        b->used = 0;
        b->capacity = cap;
        m_head = b;
    }

    LinearAllocator::Marker LinearAllocator::mark() const {
        return { m_head, m_head ? m_head->used : 0 };
    }

    void LinearAllocator::rewind(Marker m) {
        while (m_head && m_head != m.block) {
            Block* next = m_head->next;
            std::free(m_head);
            m_head = next;
        }

        if (m_head) {
            m_head->used = m.used;
        }
    }
}
