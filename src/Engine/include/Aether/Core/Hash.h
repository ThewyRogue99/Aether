//
// Created by ThewyRogue99 on 31.12.2025.
//

#pragma once

#include <cstdint>

#include "Core.h"

namespace Aether::Core {
    class StringView;

    AETHER_API uint32_t FNV1a32(const StringView& sv);
}
