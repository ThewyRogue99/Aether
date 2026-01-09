//
// Created by ThewyRogue99 on 1.01.2026.
//

#include "TempAllocator.h"

#include <Aether/Core/Memory/LinearAllocator.h>

namespace Aether::Engine {
    LinearAllocator& TempAllocator() {
        thread_local LinearAllocator allocator(256 * 1024);
        return allocator;
    }
}
