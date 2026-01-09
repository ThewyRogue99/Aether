//
// Created by ThewyRogue99 on 1.01.2026.
//

#pragma once

#include <cstddef>

#include "Aether/Core/Core.h"

namespace Aether::Engine {
    class AETHER_API BasicAllocator {
    public:
        BasicAllocator();
        ~BasicAllocator();

        BasicAllocator(const BasicAllocator&);
        BasicAllocator& operator=(const BasicAllocator&);

        void* allocate(size_t size, size_t alignment = alignof(std::max_align_t));

        void deallocate(void* ptr);
    };
}
