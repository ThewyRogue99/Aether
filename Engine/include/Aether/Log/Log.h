//
// Created by ThewyRogue99 on 5.01.2026.
//

#pragma once

#include <Aether/Log/Logger.h>
#include <Aether/Core/Memory/Pointer.h>

#if !defined(AETHER_LOG_COMPILED_LEVEL)
    #if AETHER_BUILD_DEBUG
        // Debug builds: everything
        #define AETHER_LOG_COMPILED_LEVEL ::Aether::Log::LogLevel::Trace
    #else
        // Release / Shipping builds: warnings and above
        #define AETHER_LOG_COMPILED_LEVEL ::Aether::Log::LogLevel::Warn
    #endif
#endif

#define AETHER_LOG_INTERNAL(level, ...) \
do { \
    if (static_cast<uint8_t>(level) >= \
    static_cast<uint8_t>(AETHER_LOG_COMPILED_LEVEL)) { \
        ::Aether::Log::GetLogger().Log(level, __VA_ARGS__); \
    } \
} while (0)

#define AETHER_TRACE(...)  AETHER_LOG_INTERNAL(::Aether::Log::LogLevel::Trace, __VA_ARGS__)
#define AETHER_DEBUG(...)  AETHER_LOG_INTERNAL(::Aether::Log::LogLevel::Debug, __VA_ARGS__)
#define AETHER_INFO(...)   AETHER_LOG_INTERNAL(::Aether::Log::LogLevel::Info,  __VA_ARGS__)
#define AETHER_WARN(...)   AETHER_LOG_INTERNAL(::Aether::Log::LogLevel::Warn,  __VA_ARGS__)
#define AETHER_ERROR(...)  AETHER_LOG_INTERNAL(::Aether::Log::LogLevel::Error, __VA_ARGS__)
#define AETHER_FATAL(...)  AETHER_LOG_INTERNAL(::Aether::Log::LogLevel::Fatal, __VA_ARGS__)

namespace Aether::Log {
    AETHER_API void SetLogger(Engine::Scope<Logger> logger);
    AETHER_API Logger& GetLogger();
}
