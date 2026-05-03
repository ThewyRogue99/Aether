//
// Created by ThewyRogue99 on 31.12.2025.
//

#pragma once

#include "Core.h"

#include <cstring>
#include <string_view>

namespace Aether::Engine {
    class AETHER_API StringView {
    public:
        // Sentinel "not found" value, matches std::string_view::npos semantics.
        // Returned by find/rfind when no match is found, and used as a default
        // length argument meaning "to the end".
        static constexpr uint32_t npos = static_cast<uint32_t>(-1);

        // ── Construction ─────────────────────────────────────────────
        StringView();
        StringView(const char* d, uint32_t n);
        StringView(const char* cstr);              // implicit; nullptr → empty view
        StringView(std::string_view sv);           // implicit interop with std

        static StringView FromCString(const char* cstr);

        // ── Capacity ─────────────────────────────────────────────────
        [[nodiscard]] uint32_t size() const;
        [[nodiscard]] uint32_t length() const;     // alias for size()
        [[nodiscard]] bool empty() const;

        // ── Element access ───────────────────────────────────────────
        [[nodiscard]] const char* data() const;
        [[nodiscard]] char operator[](uint32_t i) const;
        [[nodiscard]] char at(uint32_t i) const;   // bounds-checked
        [[nodiscard]] char front() const;
        [[nodiscard]] char back() const;

        // ── Iterators ────────────────────────────────────────────────
        [[nodiscard]] const char* begin() const;
        [[nodiscard]] const char* end() const;
        [[nodiscard]] const char* cbegin() const;
        [[nodiscard]] const char* cend() const;

        // ── Modifiers (re-aim the view; the underlying data isn't changed) ──
        void remove_prefix(uint32_t n);
        void remove_suffix(uint32_t n);

        // ── Substring / search ───────────────────────────────────────
        [[nodiscard]] StringView substr(uint32_t pos = 0, uint32_t count = npos) const;

        [[nodiscard]] uint32_t find(char c, uint32_t pos = 0) const;
        [[nodiscard]] uint32_t find(StringView needle, uint32_t pos = 0) const;
        [[nodiscard]] uint32_t rfind(char c, uint32_t pos = npos) const;
        [[nodiscard]] uint32_t rfind(StringView needle, uint32_t pos = npos) const;

        [[nodiscard]] bool starts_with(StringView prefix) const;
        [[nodiscard]] bool starts_with(char c) const;
        [[nodiscard]] bool ends_with(StringView suffix) const;
        [[nodiscard]] bool ends_with(char c) const;
        [[nodiscard]] bool contains(StringView needle) const;
        [[nodiscard]] bool contains(char c) const;

        // ── Comparison ───────────────────────────────────────────────
        [[nodiscard]] int compare(StringView other) const;

        friend bool operator==(StringView a, StringView b);
        friend bool operator!=(StringView a, StringView b);
        friend bool operator< (StringView a, StringView b);
        friend bool operator<=(StringView a, StringView b);
        friend bool operator> (StringView a, StringView b);
        friend bool operator>=(StringView a, StringView b);

        // ── Conversion to std::string_view (implicit) ────────────────
        operator std::string_view() const { return std::string_view(m_Data, m_Size); }

    private:
        const char* m_Data = nullptr;
        uint32_t    m_Size = 0;
    };
}
