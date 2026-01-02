//
// Created by ThewyRogue99 on 31.12.2025.
//

#pragma once

#include <cassert>

#include "Core.h"

#if AETHER_DEBUG_BUILD
    #define AETHER_ASSERT(x) do { if (!(x)) { AETHER_DEBUGBREAK(); assert(x); } } while (0)
#else
    #define AETHER_ASSERT(x) ((void)0)
#endif
