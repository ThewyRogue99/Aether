//
// Created by ThewyRogue99 on 1.01.2026.
//

#include "NameAllocator.h"

#include <Aether/Core/Memory/LinearAllocator.h>

namespace Aether::Engine {
    class LinearAllocator;

    LinearAllocator& NameAllocator() {
        static LinearAllocator alloc(512 * 1024); // grows as needed
        return alloc;
    }
}
