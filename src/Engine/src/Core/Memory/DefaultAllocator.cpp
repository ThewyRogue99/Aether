//
// Created by ThewyRogue99 on 1.01.2026.
//

#include "DefaultAllocator.h"

#include <Aether/Core/Memory/BasicAllocator.h>

namespace Aether::Core {
    BasicAllocator& DefaultAllocator() {
        thread_local BasicAllocator allocator;
        return allocator;
    }
}
