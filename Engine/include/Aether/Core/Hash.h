//
// Created by ThewyRogue99 on 31.12.2025.
//

#pragma once

#include "Core.h"
#include "Aether/Core/StringView.h"

namespace Aether::Engine {
    AETHER_API uint32_t FNV1a32(const StringView& sv);
}
