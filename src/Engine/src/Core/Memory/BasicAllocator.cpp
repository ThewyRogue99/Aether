//
// Created by ThewyRogue99 on 1.01.2026.
//

#include <Aether/Core/Memory/BasicAllocator.h>

#include <Aether/Core/Assert.h>
#include <new>

constexpr bool isPowerOfTwo(uint32_t x) {
    return x != 0 && (x & (x - 1)) == 0;
}

namespace Aether::Engine {
    BasicAllocator::BasicAllocator() = default;
    BasicAllocator::~BasicAllocator() = default;

    BasicAllocator::BasicAllocator(const BasicAllocator&) = default;
    BasicAllocator& BasicAllocator::operator=(const BasicAllocator&) = default;

    void* BasicAllocator::allocate(size_t size, size_t alignment) {
        AETHER_ASSERT(size > 0);
        AETHER_ASSERT(isPowerOfTwo(alignment));

#if defined(_MSC_VER)
        void* ptr = _aligned_malloc(size, alignment);
        AETHER_ASSERT_MSG(ptr, "BasicAllocator: allocation failed");
        return ptr;
#else
        void* ptr = nullptr;
        int res = posix_memalign(&ptr, alignment, size);
        AETHER_ASSERT_MSG(res == 0 && ptr, "BasicAllocator: allocation failed");
        return ptr;
#endif
    }

    void BasicAllocator::deallocate(void* ptr) {
        if (!ptr) return;

#if defined(_MSC_VER)
        _aligned_free(ptr);
#else
        std::free(ptr);
#endif
    }
}
