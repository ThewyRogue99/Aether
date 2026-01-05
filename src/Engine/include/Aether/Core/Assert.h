//
// Created by ThewyRogue99 on 31.12.2025.
//

#pragma once

#include <cassert>

#include "Core.h"

#if AETHER_BUILD_DEBUG
    #define AETHER_ASSERT(x) \
    do { \
        if (!(x)) { \
            AETHER_DEBUGBREAK(); \
            std::abort(); \
        } \
    } while (0)
#else
    #define AETHER_ASSERT(x) ((void)0)
#endif
