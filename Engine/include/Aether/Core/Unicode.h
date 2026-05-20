//
// Created by ThewyRogue99 on 15.05.2026.
//

#pragma once

#include <Aether/Core/Path.h>
#include <Aether/Core/StringView.h>

namespace Aether::Engine {
    // ── Low-level ─────────────────────────────────────────────────────────────────

    [[nodiscard]] WString Utf8ToWide(StringView utf8);

    [[nodiscard]] String WideToUtf8(WStringView wide);

    // ── Path helpers ──────────────────────────────────────────────────────────────

    [[nodiscard]] Path PathFromUtf8(StringView utf8);

    [[nodiscard]] String PathToUtf8(PathView path);
} // namespace Aether::Engine
