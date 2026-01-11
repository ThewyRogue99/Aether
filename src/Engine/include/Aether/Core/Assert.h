//
// Created by ThewyRogue99 on 31.12.2025.
//

#pragma once

#include <cassert>

#include <Aether/Core/Core.h>
#include <Aether/Log/Log.h>

#if AETHER_BUILD_DEBUG
    #define AETHER_INTERNAL_ASSERT_IMPL(cond, msg, ...) \
        do { \
            if (!(cond)) { \
                AETHER_ERROR(msg, ##__VA_ARGS__); \
                AETHER_DEBUGBREAK(); \
                std::abort(); \
            } \
        } while (0)

    #define AETHER_INTERNAL_ASSERT_NO_MSG(cond) \
        do { \
            if (!(cond)) { \
                AETHER_ERROR( \
                "Assertion failed: %s", #cond); \
                AETHER_DEBUGBREAK(); \
                std::abort(); \
            } \
        } while (0)

    #define AETHER_INTERNAL_ASSERT_GET_MACRO(_1, _2, NAME, ...) NAME

    #define AETHER_ASSERT(...) \
        AETHER_INTERNAL_ASSERT_GET_MACRO( \
            __VA_ARGS__, \
            AETHER_INTERNAL_ASSERT_IMPL, \
            AETHER_INTERNAL_ASSERT_NO_MSG \
        )(__VA_ARGS__)

#else
    #define AETHER_ASSERT(...) ((void)0)
#endif
