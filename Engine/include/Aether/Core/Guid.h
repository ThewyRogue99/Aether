//
// Created by ThewyRogue99 on 15.05.2026.
//

#pragma once

#include <cstdint>
#include <functional>

#include <Aether/Core/Core.h>
#include <Aether/Core/String.h>
#include <Aether/Core/StringView.h>

namespace Aether::Engine {
    // ─────────────────────────────────────────────────────────────────────────────
    // Guid — 128-bit universally unique identifier (RFC 4122 version 4, random).
    //
    // Trivially copyable, 16 bytes.
    // ─────────────────────────────────────────────────────────────────────────────
    struct AETHER_API Guid {
        uint64_t hi = 0;  // upper 64 bits  (time_low | time_mid | time_hi_ver)
        uint64_t lo = 0;  // lower 64 bits  (clock_seq | node)

        // Returns a new random v4 UUID.
        [[nodiscard]] static Guid Generate();

        // Parse a UUID string in 8-4-4-4-12 hex form.
        // Returns an invalid (all-zero) Guid on parse failure.
        [[nodiscard]] static Guid FromString(StringView sv);

        // Format as "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx".
        [[nodiscard]] String ToString() const;

        // A Guid is valid when it is non-zero.
        [[nodiscard]] bool IsValid() const { return hi != 0 || lo != 0; }

        bool operator==(const Guid& o) const { return hi == o.hi && lo == o.lo; }
        bool operator!=(const Guid& o) const { return !(*this == o); }
        bool operator< (const Guid& o) const { return hi < o.hi || (hi == o.hi && lo < o.lo); }
    };

    // Deterministic Guid derived from a virtual path string (FNV-1a over the UTF-8
    // bytes). Used as a temporary stable identifier for assets that don't yet have
    // a proper binary envelope header (legacy-JSON fallback).
    [[nodiscard]] AETHER_API Guid GuidFromPath(StringView virtualPath);

} // namespace Aether::Engine

// ── std::hash specialisation ──────────────────────────────────────────────────
template<>
struct std::hash<Aether::Engine::Guid> {
    std::size_t operator()(const Aether::Engine::Guid& g) const noexcept {
        // Combine the two 64-bit halves with a simple mixing step.
        std::size_t h = g.hi;
        h ^= g.lo + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        return h;
    }
};
