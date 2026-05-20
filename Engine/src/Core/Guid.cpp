//
// Created by ThewyRogue99 on 15.05.2026.
//

#include <Aether/Core/Guid.h>

#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <random>

namespace Aether::Engine {

    // ─────────────────────────────────────────────────────────────────────────────
    // Generation
    // ─────────────────────────────────────────────────────────────────────────────

    Guid Guid::Generate() {
        // One PRNG per thread, seeded from the system entropy source.
        thread_local static std::mt19937_64 rng([]() -> uint64_t {
            std::random_device rd;
            // Fold two 32-bit words for platforms where random_device returns 32 bits.
            return (static_cast<uint64_t>(rd()) << 32) ^ rd();
        }());

        Guid g;
        g.hi = rng();
        g.lo = rng();

        // RFC 4122 §4.4 — set version = 4 (bits [15:12] of time_hi_and_version,
        // which occupies the lowest 16 bits of hi).
        g.hi = (g.hi & 0xFFFFFFFFFFFF0FFFull) | 0x0000000000004000ull;

        // RFC 4122 §4.4 — set variant = 10xx in the two most-significant bits of lo.
        g.lo = (g.lo & 0x3FFFFFFFFFFFFFFFull) | 0x8000000000000000ull;

        return g;
    }

    // ─────────────────────────────────────────────────────────────────────────────
    // String formatting: "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx"
    // ─────────────────────────────────────────────────────────────────────────────

    String Guid::ToString() const {
        // Break out the five RFC 4122 fields from hi/lo.
        const uint32_t timeLow   = static_cast<uint32_t>(hi >> 32);
        const uint16_t timeMid   = static_cast<uint16_t>(hi >> 16);
        const uint16_t timeHiVer = static_cast<uint16_t>(hi);
        const uint16_t clockSeq  = static_cast<uint16_t>(lo >> 48);
        const uint64_t node      = lo & 0x0000FFFFFFFFFFFFull;

        char buf[37]; // 32 hex digits + 4 dashes + null
        std::snprintf(buf, sizeof(buf),
                      "%08x-%04x-%04x-%04x-%012llx",
                      timeLow, timeMid, timeHiVer, clockSeq,
                      static_cast<unsigned long long>(node));
        return String(buf);
    }

    // ─────────────────────────────────────────────────────────────────────────────
    // Parsing
    // ─────────────────────────────────────────────────────────────────────────────

    static uint64_t ParseHex(const char* src, int digits, bool& ok) {
        uint64_t result = 0;
        for (int i = 0; i < digits; ++i) {
            char c = src[i];
            uint64_t nibble;
            if (c >= '0' && c <= '9')      nibble = static_cast<uint64_t>(c - '0');
            else if (c >= 'a' && c <= 'f') nibble = static_cast<uint64_t>(c - 'a' + 10);
            else if (c >= 'A' && c <= 'F') nibble = static_cast<uint64_t>(c - 'A' + 10);
            else { ok = false; return 0; }
            result = (result << 4) | nibble;
        }
        return result;
    }

    Guid Guid::FromString(StringView sv) {
        // Expected layout: "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
        //                   01234567 8 9012 3 4567 8 9012 3 456789012345
        //                   ^8      ^-^4   ^-^4   ^-^4   ^-^12
        // Total length = 36
        if (sv.size() != 36) return {};
        const char* s = sv.data();

        if (s[8] != '-' || s[13] != '-' || s[18] != '-' || s[23] != '-') return {};

        bool ok = true;
        const uint64_t timeLow   = ParseHex(s,       8,  ok);
        const uint64_t timeMid   = ParseHex(s + 9,   4,  ok);
        const uint64_t timeHiVer = ParseHex(s + 14,  4,  ok);
        const uint64_t clockSeq  = ParseHex(s + 19,  4,  ok);
        const uint64_t node      = ParseHex(s + 24,  12, ok);
        if (!ok) return {};

        Guid g;
        g.hi = (timeLow << 32) | (timeMid << 16) | timeHiVer;
        g.lo = (clockSeq << 48) | node;
        return g;
    }

    // ─────────────────────────────────────────────────────────────────────────────
    // Deterministic Guid from a virtual path
    // ─────────────────────────────────────────────────────────────────────────────

    Guid GuidFromPath(StringView virtualPath) {
        // FNV-1a 64-bit, applied twice with different seeds to produce 128 bits.
        constexpr uint64_t kFNV1aBasis  = 0xcbf29ce484222325ull;
        constexpr uint64_t kFNV1aPrime  = 0x00000100000001b3ull;
        constexpr uint64_t kSeed2       = 0xdeadbeefcafebabeull;

        uint64_t hi = kFNV1aBasis;
        uint64_t lo = kFNV1aBasis ^ kSeed2;

        for (char c : virtualPath) {
            hi ^= static_cast<uint64_t>(static_cast<uint8_t>(c));
            hi *= kFNV1aPrime;
            lo ^= static_cast<uint64_t>(static_cast<uint8_t>(c));
            lo *= kFNV1aPrime;
        }

        return Guid{ hi, lo };
    }

} // namespace Aether::Engine
