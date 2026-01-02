//
// Created by ThewyRogue99 on 26.12.2025.
//

#pragma once

// ============================================================
// Platform detection
// ============================================================

#if defined(_WIN32) || defined(_WIN64)
    #define AETHER_PLATFORM_WINDOWS 1
#elif defined(__APPLE__)
    #define AETHER_PLATFORM_MACOS 1
#elif defined(__linux__)
    #define AETHER_PLATFORM_LINUX 1
#else
    #error "Unsupported platform"
#endif

// ============================================================
// Build configuration
// ============================================================

#if defined(_DEBUG) || defined(AETHER_DEBUG)
    #define AETHER_DEBUG_BUILD 1
#else
    #define AETHER_RELEASE_BUILD 1
#endif

// ============================================================
// DLL import / export
// ============================================================

// When building the engine itself, define AETHER_BUILD_DLL
#if AETHER_PLATFORM_WINDOWS
    #if defined(AETHER_BUILD_DLL)
        #define AETHER_API __declspec(dllexport)
    #else
        #define AETHER_API __declspec(dllimport)
    #endif
#else
    // On Linux / macOS, visibility attributes are optional
    #define AETHER_API
#endif

// ============================================================
// Force inline
// ============================================================

#if defined(_MSC_VER)
    #define AETHER_FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
    #define AETHER_FORCE_INLINE inline __attribute__((always_inline))
#else
    #define AETHER_FORCE_INLINE inline
#endif

// ============================================================
// Debug break
// ============================================================

#if AETHER_PLATFORM_WINDOWS
    #define AETHER_DEBUGBREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
    #define AETHER_DEBUGBREAK() __builtin_trap()
#else
    #define AETHER_DEBUGBREAK()
#endif

// ============================================================
// Utility macros
// ============================================================

#define AETHER_UNUSED(x) (void)(x)
#define AETHER_STRINGIFY(x) #x
#define AETHER_EXPAND(x) x
