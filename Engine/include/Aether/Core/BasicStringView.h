//
// Created by ThewyRogue99 on 15.05.2026.
//

#pragma once

#include <Aether/Core/Assert.h>

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>

namespace Aether::Engine {
    template<typename CharT>
    class BasicStringView {
    public:
        using CharType = CharT;
        using Traits = std::char_traits<CharT>;

        // Sentinel "not found" value.
        static constexpr uint32_t npos = static_cast<uint32_t>(-1);

        // ── Construction ──────────────────────────────────────────────────────────

        constexpr BasicStringView() noexcept = default;

        constexpr BasicStringView(const CharT* d, uint32_t n) noexcept
            : m_Data(d), m_Size(n) {}

        // Implicit construction from a null-terminated string.
        constexpr BasicStringView(const CharT* cstr) noexcept  // NOLINT(google-explicit-constructor)
            : m_Data(cstr),
              m_Size(cstr ? static_cast<uint32_t>(Traits::length(cstr)) : 0) {}

        // Implicit interop with std::basic_string_view<CharT>.
        constexpr BasicStringView(std::basic_string_view<CharT> sv) noexcept  // NOLINT(google-explicit-constructor)
            : m_Data(sv.data()), m_Size(static_cast<uint32_t>(sv.size())) {}

        static BasicStringView FromCString(const CharT* cstr) noexcept {
            if (!cstr) return {};
            return { cstr, static_cast<uint32_t>(Traits::length(cstr)) };
        }

        // ── Capacity ──────────────────────────────────────────────────────────────

        [[nodiscard]] constexpr uint32_t size() const noexcept { return m_Size; }
        [[nodiscard]] constexpr uint32_t length() const noexcept { return m_Size; }
        [[nodiscard]] constexpr bool empty() const noexcept { return m_Size == 0; }

        // ── Element access ────────────────────────────────────────────────────────

        [[nodiscard]] constexpr const CharT* data() const noexcept { return m_Data; }

        [[nodiscard]] CharT operator[](uint32_t i) const {
            AETHER_ASSERT(i < m_Size);
            return m_Data[i];
        }

        [[nodiscard]] CharT at(uint32_t i) const {
            AETHER_ASSERT_MSG(i < m_Size, "BasicStringView::at out of range");
            return m_Data[i];
        }

        [[nodiscard]] CharT front() const {
            AETHER_ASSERT_MSG(m_Size > 0, "BasicStringView::front on empty view");
            return m_Data[0];
        }

        [[nodiscard]] CharT back() const {
            AETHER_ASSERT_MSG(m_Size > 0, "BasicStringView::back on empty view");
            return m_Data[m_Size - 1];
        }

        // ── Iterators ─────────────────────────────────────────────────────────────

        [[nodiscard]] constexpr const CharT* begin() const noexcept { return m_Data; }
        [[nodiscard]] constexpr const CharT* end() const noexcept { return m_Data + m_Size; }
        [[nodiscard]] constexpr const CharT* cbegin() const noexcept { return m_Data; }
        [[nodiscard]] constexpr const CharT* cend() const noexcept { return m_Data + m_Size; }

        // ── Modifiers (re-aim the view; underlying data is unchanged) ─────────────

        void remove_prefix(uint32_t n) {
            AETHER_ASSERT_MSG(n <= m_Size, "BasicStringView::remove_prefix beyond view");
            m_Data += n;
            m_Size -= n;
        }

        void remove_suffix(uint32_t n) {
            AETHER_ASSERT_MSG(n <= m_Size, "BasicStringView::remove_suffix beyond view");
            m_Size -= n;
        }

        // ── Substring / search ────────────────────────────────────────────────────

        [[nodiscard]] BasicStringView substr(uint32_t pos = 0, uint32_t count = npos) const {
            AETHER_ASSERT_MSG(pos <= m_Size, "BasicStringView::substr pos out of range");
            const uint32_t remaining = m_Size - pos;
            const uint32_t len = (count < remaining) ? count : remaining;
            return { m_Data + pos, len };
        }

        [[nodiscard]] uint32_t find(CharT c, uint32_t pos = 0) const noexcept {
            for (uint32_t i = pos; i < m_Size; ++i)
                if (Traits::eq(m_Data[i], c)) return i;
            return npos;
        }

        [[nodiscard]] uint32_t find(BasicStringView needle, uint32_t pos = 0) const noexcept {
            if (needle.m_Size == 0) return (pos <= m_Size) ? pos : npos;
            if (needle.m_Size > m_Size) return npos;
            const uint32_t lastStart = m_Size - needle.m_Size;
            for (uint32_t i = pos; i <= lastStart; ++i)
                if (Traits::compare(m_Data + i, needle.m_Data, needle.m_Size) == 0) return i;
            return npos;
        }

        [[nodiscard]] uint32_t rfind(CharT c, uint32_t pos = npos) const noexcept {
            if (m_Size == 0) return npos;
            const uint32_t start = (pos >= m_Size) ? m_Size - 1 : pos;
            for (uint32_t i = start + 1; i-- > 0;)
                if (Traits::eq(m_Data[i], c)) return i;
            return npos;
        }

        [[nodiscard]] uint32_t rfind(BasicStringView needle, uint32_t pos = npos) const noexcept {
            if (needle.m_Size == 0) return (pos < m_Size) ? pos : m_Size;
            if (needle.m_Size > m_Size) return npos;
            const uint32_t lastStart = m_Size - needle.m_Size;
            const uint32_t start = (pos > lastStart) ? lastStart : pos;
            for (uint32_t i = start + 1; i-- > 0;)
                if (Traits::compare(m_Data + i, needle.m_Data, needle.m_Size) == 0) return i;
            return npos;
        }

        [[nodiscard]] bool starts_with(BasicStringView prefix) const noexcept {
            if (prefix.m_Size > m_Size) return false;
            return Traits::compare(m_Data, prefix.m_Data, prefix.m_Size) == 0;
        }

        [[nodiscard]] bool starts_with(CharT c) const noexcept {
            return m_Size > 0 && Traits::eq(m_Data[0], c);
        }

        [[nodiscard]] bool ends_with(BasicStringView suffix) const noexcept {
            if (suffix.m_Size > m_Size) return false;
            return Traits::compare(m_Data + (m_Size - suffix.m_Size), suffix.m_Data, suffix.m_Size) == 0;
        }

        [[nodiscard]] bool ends_with(CharT c) const noexcept {
            return m_Size > 0 && Traits::eq(m_Data[m_Size - 1], c);
        }

        [[nodiscard]] bool contains(BasicStringView needle) const noexcept { return find(needle) != npos; }
        [[nodiscard]] bool contains(CharT c) const noexcept { return find(c) != npos; }

        // ── Comparison ────────────────────────────────────────────────────────────

        [[nodiscard]] int compare(BasicStringView other) const noexcept {
            const uint32_t n = (m_Size < other.m_Size) ? m_Size : other.m_Size;
            if (n > 0) {
                const int r = Traits::compare(m_Data, other.m_Data, n);
                if (r != 0) return r;
            }
            if (m_Size == other.m_Size) return 0;
            return (m_Size < other.m_Size) ? -1 : 1;
        }

        friend bool operator==(BasicStringView a, BasicStringView b) noexcept {
            return a.m_Size == b.m_Size &&
                   (a.m_Size == 0 || Traits::compare(a.m_Data, b.m_Data, a.m_Size) == 0);
        }
        friend bool operator!=(BasicStringView a, BasicStringView b) noexcept { return !(a == b); }
        friend bool operator< (BasicStringView a, BasicStringView b) noexcept { return a.compare(b) <  0; }
        friend bool operator<=(BasicStringView a, BasicStringView b) noexcept { return a.compare(b) <= 0; }
        friend bool operator> (BasicStringView a, BasicStringView b) noexcept { return a.compare(b) >  0; }
        friend bool operator>=(BasicStringView a, BasicStringView b) noexcept { return a.compare(b) >= 0; }

        // ── Conversion to std::basic_string_view<CharT> ───────────────────────────

        operator std::basic_string_view<CharT>() const noexcept {  // NOLINT(google-explicit-constructor)
            return std::basic_string_view<CharT>(m_Data, m_Size);
        }

    private:
        const CharT* m_Data = nullptr;
        uint32_t m_Size = 0;
    };
} // namespace Aether::Engine

// ── std::hash specialisation (covers all BasicStringView<CharT>) ──────────────
template<typename CharT>
struct std::hash<Aether::Engine::BasicStringView<CharT>> {
    std::size_t operator()(const Aether::Engine::BasicStringView<CharT>& sv) const noexcept {
        return std::hash<std::basic_string_view<CharT>>{}(
            static_cast<std::basic_string_view<CharT>>(sv)
        );
    }
};
