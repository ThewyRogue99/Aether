//
// Created by ThewyRogue99 on 31.12.2025.
//

#pragma once

#include <cassert>

#include <Aether/Core/Core.h>
#include <Aether/Log/Log.h>

#if AETHER_BUILD_DEBUG
    #define AETHER_ASSERT(cond) \
    do { \
        if (!(cond)) { \
            AETHER_ERROR("Assertion failed: %s", #cond); \
            AETHER_DEBUGBREAK(); \
            std::abort(); \
        } \
    } while (0)

    #define AETHER_ASSERT_MSG(cond, fmt, ...) \
    do { \
        if (!(cond)) { \
            AETHER_ERROR(fmt, ##__VA_ARGS__); \
            AETHER_DEBUGBREAK(); \
            std::abort(); \
        } \
    } while (0)
#else
    #define AETHER_ASSERT(cond) ((void)0)
    #define AETHER_ASSERT_MSG(cond, ...) ((void)0)
#endif
