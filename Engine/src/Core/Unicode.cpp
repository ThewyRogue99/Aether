//
// Created by Aether on 15.05.2026.
//

#include <Aether/Core/Unicode.h>

// Full template bodies are required here.
#include <Aether/Core/String.h>
#include <Aether/Log/Log.h>

#if defined(AETHER_PLATFORM_WINDOWS)
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
    #define NOMINMAX
    #endif
    #include <Windows.h>
#endif

namespace Aether::Engine {

    // ─────────────────────────────────────────────────────────────────────────────
    // Platform-specific implementations
    // ─────────────────────────────────────────────────────────────────────────────

    #if defined(AETHER_PLATFORM_WINDOWS)

    // Windows — delegate to the OS which handles all edge cases correctly.

    WString Utf8ToWide(StringView utf8) {
        if (utf8.empty()) return {};

        const int srcLen = static_cast<int>(utf8.size());
        const int needed = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                                                 utf8.data(), srcLen,
                                                 nullptr, 0);
        if (needed <= 0) {
            AETHER_ERROR("Utf8ToWide: invalid UTF-8 sequence (error %lu)", ::GetLastError());
            return {};
        }

        WString result;
        result.resize(static_cast<uint32_t>(needed));
        ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                              utf8.data(), srcLen,
                              result.data(), needed);
        return result;
    }

    String WideToUtf8(WStringView wide) {
        if (wide.empty()) return {};

        const int srcLen = static_cast<int>(wide.size());
        const int needed = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                                                 wide.data(), srcLen,
                                                 nullptr, 0,
                                                 nullptr, nullptr);
        if (needed <= 0) {
            AETHER_ERROR("WideToUtf8: invalid UTF-16 sequence (error %lu)", ::GetLastError());
            return {};
        }

        String result;
        result.resize(static_cast<uint32_t>(needed));
        ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                              wide.data(), srcLen,
                              result.data(), needed,
                              nullptr, nullptr);
        return result;
    }

    #else

    // Linux / macOS — hand-rolled UTF-8 ↔ UTF-16 (codepoint-by-codepoint).
    // Handles the full Unicode range including supplementary characters (surrogate pairs
    // in UTF-16). Invalid sequences produce an empty result and an error log.

    // Decode one UTF-8 codepoint from [src, end). Returns the next read position,
    // or nullptr on error (writes U+FFFD replacement on error when replacement is true).
    static const char* DecodeUtf8(const char* src, const char* end, uint32_t& cp) {
        if (src >= end) { cp = 0; return nullptr; }

        const auto b0 = static_cast<uint8_t>(*src);

        if (b0 < 0x80) {
            // 1-byte sequence: 0xxxxxxx
            cp = b0;
            return src + 1;
        }
        if (b0 < 0xC2) {
            // Overlong or continuation byte — invalid.
            AETHER_ERROR("Utf8ToWide: invalid leading byte 0x%02X", b0);
            return nullptr;
        }

        int extra;
        uint32_t minCp;
        if      (b0 < 0xE0) { extra = 1; cp = b0 & 0x1F; minCp = 0x0080; }
        else if (b0 < 0xF0) { extra = 2; cp = b0 & 0x0F; minCp = 0x0800; }
        else if (b0 < 0xF5) { extra = 3; cp = b0 & 0x07; minCp = 0x10000; }
        else {
            AETHER_ERROR("Utf8ToWide: codepoint out of Unicode range (leading byte 0x%02X)", b0);
            return nullptr;
        }

        if (src + extra >= end) {
            AETHER_ERROR("Utf8ToWide: truncated UTF-8 sequence");
            return nullptr;
        }

        for (int i = 0; i < extra; ++i) {
            const auto bN = static_cast<uint8_t>(src[1 + i]);
            if ((bN & 0xC0) != 0x80) {
                AETHER_ERROR("Utf8ToWide: invalid continuation byte 0x%02X", bN);
                return nullptr;
            }
            cp = (cp << 6) | (bN & 0x3F);
        }

        if (cp < minCp) {
            AETHER_ERROR("Utf8ToWide: overlong encoding (codepoint U+%04X)", cp);
            return nullptr;
        }
        if (cp >= 0xD800 && cp <= 0xDFFF) {
            AETHER_ERROR("Utf8ToWide: lone surrogate U+%04X in UTF-8 is invalid", cp);
            return nullptr;
        }
        if (cp > 0x10FFFF) {
            AETHER_ERROR("Utf8ToWide: codepoint U+%06X out of range", cp);
            return nullptr;
        }

        return src + 1 + extra;
    }

    WString Utf8ToWide(StringView utf8) {
        if (utf8.empty()) return {};

        WString result;
        result.reserve(static_cast<uint32_t>(utf8.size())); // rough upper bound

        const char* p   = utf8.data();
        const char* end = p + utf8.size();

        while (p < end) {
            uint32_t cp = 0;
            p = DecodeUtf8(p, end, cp);
            if (!p) return {};

            if (cp <= 0xFFFF) {
                // BMP codepoint — one wchar_t on both 16-bit and 32-bit platforms.
                result.push_back(static_cast<wchar_t>(cp));
            } else {
                // Supplementary character — encode as a surrogate pair on 16-bit wchar_t,
                // or as a single 32-bit wchar_t on Linux/macOS where sizeof(wchar_t) == 4.
                if constexpr (sizeof(wchar_t) == 2) {
                    // UTF-16 surrogate pair.
                    cp -= 0x10000;
                    result.push_back(static_cast<wchar_t>(0xD800 | (cp >> 10)));
                    result.push_back(static_cast<wchar_t>(0xDC00 | (cp & 0x3FF)));
                } else {
                    result.push_back(static_cast<wchar_t>(cp));
                }
            }
        }
        return result;
    }

    // Decode one UTF-16 codepoint from [src, end).  Returns next position or nullptr.
    static const wchar_t* DecodeUtf16(const wchar_t* src, const wchar_t* end, uint32_t& cp) {
        if (src >= end) { cp = 0; return nullptr; }

        const auto w0 = static_cast<uint16_t>(*src);

        if constexpr (sizeof(wchar_t) == 4) {
            // 32-bit wchar_t — each element is a full codepoint.
            cp = static_cast<uint32_t>(*src);
            if (cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF)) {
                AETHER_ERROR("WideToUtf8: invalid codepoint U+%06X", cp);
                return nullptr;
            }
            return src + 1;
        }

        // 16-bit wchar_t — handle surrogate pairs.
        if (w0 >= 0xD800 && w0 <= 0xDBFF) {
            // High surrogate — expect a low surrogate next.
            if (src + 1 >= end) {
                AETHER_ERROR("WideToUtf8: lone high surrogate at end of string");
                return nullptr;
            }
            const auto w1 = static_cast<uint16_t>(src[1]);
            if (w1 < 0xDC00 || w1 > 0xDFFF) {
                AETHER_ERROR("WideToUtf8: high surrogate not followed by low surrogate");
                return nullptr;
            }
            cp = 0x10000 + (static_cast<uint32_t>(w0 - 0xD800) << 10) + (w1 - 0xDC00);
            return src + 2;
        }
        if (w0 >= 0xDC00 && w0 <= 0xDFFF) {
            AETHER_ERROR("WideToUtf8: lone low surrogate 0x%04X", w0);
            return nullptr;
        }
        cp = w0;
        return src + 1;
    }

    String WideToUtf8(WStringView wide) {
        if (wide.empty()) return {};

        String result;
        result.reserve(static_cast<uint32_t>(wide.size() * 3)); // rough upper bound

        const wchar_t* p   = wide.data();
        const wchar_t* end = p + wide.size();

        while (p < end) {
            uint32_t cp = 0;
            p = DecodeUtf16(p, end, cp);
            if (!p) return {};

            // Encode cp as UTF-8.
            if (cp < 0x80) {
                result.push_back(static_cast<char>(cp));
            } else if (cp < 0x800) {
                result.push_back(static_cast<char>(0xC0 | (cp >> 6)));
                result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
            } else if (cp < 0x10000) {
                result.push_back(static_cast<char>(0xE0 | (cp >> 12)));
                result.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
                result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
            } else {
                result.push_back(static_cast<char>(0xF0 | (cp >> 18)));
                result.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
                result.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
                result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
            }
        }
        return result;
    }

    #endif // AETHER_PLATFORM_WINDOWS

    // ─────────────────────────────────────────────────────────────────────────────
    // Path helpers — platform-independent interface
    // ─────────────────────────────────────────────────────────────────────────────

    Path PathFromUtf8(StringView utf8) {
    #if defined(AETHER_PLATFORM_WINDOWS)
        return Utf8ToWide(utf8);
    #else
        return Path(utf8);
    #endif
    }

    String PathToUtf8(PathView path) {
    #if defined(AETHER_PLATFORM_WINDOWS)
        return WideToUtf8(path);
    #else
        return String(path);
    #endif
    }

} // namespace Aether::Engine
