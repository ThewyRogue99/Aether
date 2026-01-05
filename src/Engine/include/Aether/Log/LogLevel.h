//
// Created by ThewyRogue99 on 5.01.2026.
//

#pragma once

#include <cstdint>
#include <Aether/Core/Core.h>

namespace Aether::Log {
    enum class AETHER_API LogLevel : uint8_t {
        Trace = 0,
        Debug = 1,
        Info  = 2,
        Warn  = 3,
        Error = 4,
        Fatal = 5,
        Off   = 6
    };
}
