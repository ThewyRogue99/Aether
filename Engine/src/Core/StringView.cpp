//
// Created by ThewyRogue99 on 31.12.2025.
//

#include <Aether/Core/StringView.h>

#include <Aether/Core/Assert.h>

namespace Aether::Engine {
    // ── Construction ─────────────────────────────────────────────────
    StringView::StringView() = default;

    StringView::StringView(const char* d, uint32_t n)
        : m_Data(d), m_Size(n) { }

    StringView::StringView(const char* cstr)
        : m_Data(cstr),
          m_Size(cstr ? static_cast<uint32_t>(std::strlen(cstr)) : 0) { }

    StringView::StringView(std::string_view sv)
        : m_Data(sv.data()), m_Size(static_cast<uint32_t>(sv.size())) { }

    StringView StringView::FromCString(const char* cstr) {
        if (!cstr) return {};
        return StringView{ cstr, static_cast<uint32_t>(std::strlen(cstr)) };
    }

    // ── Capacity ─────────────────────────────────────────────────────
    uint32_t StringView::size() const   { return m_Size; }
    uint32_t StringView::length() const { return m_Size; }
    bool StringView::empty() const      { return m_Size == 0; }

    // ── Element access ───────────────────────────────────────────────
    const char* StringView::data() const { return m_Data; }

    char StringView::operator[](uint32_t i) const {
        AETHER_ASSERT(i < m_Size);
        return m_Data[i];
    }

    char StringView::at(uint32_t i) const {
        AETHER_ASSERT_MSG(i < m_Size, "StringView::at out of range");
        return m_Data[i];
    }

    char StringView::front() const {
        AETHER_ASSERT_MSG(m_Size > 0, "StringView::front on empty view");
        return m_Data[0];
    }

    char StringView::back() const {
        AETHER_ASSERT_MSG(m_Size > 0, "StringView::back on empty view");
        return m_Data[m_Size - 1];
    }

    // ── Iterators ────────────────────────────────────────────────────
    const char* StringView::begin()  const { return m_Data; }
    const char* StringView::end()    const { return m_Data + m_Size; }
    const char* StringView::cbegin() const { return m_Data; }
    const char* StringView::cend()   const { return m_Data + m_Size; }

    // ── Modifiers ────────────────────────────────────────────────────
    void StringView::remove_prefix(uint32_t n) {
        AETHER_ASSERT_MSG(n <= m_Size, "remove_prefix beyond view");
        m_Data += n;
        m_Size -= n;
    }

    void StringView::remove_suffix(uint32_t n) {
        AETHER_ASSERT_MSG(n <= m_Size, "remove_suffix beyond view");
        m_Size -= n;
    }

    // ── Substring / search ───────────────────────────────────────────
    StringView StringView::substr(uint32_t pos, uint32_t count) const {
        AETHER_ASSERT_MSG(pos <= m_Size, "substr pos out of range");
        const uint32_t remaining = m_Size - pos;
        const uint32_t len = count < remaining ? count : remaining;
        return StringView(m_Data + pos, len);
    }

    uint32_t StringView::find(char c, uint32_t pos) const {
        for (uint32_t i = pos; i < m_Size; ++i) {
            if (m_Data[i] == c) return i;
        }
        return npos;
    }

    uint32_t StringView::find(StringView needle, uint32_t pos) const {
        if (needle.m_Size == 0) {
            return pos <= m_Size ? pos : npos;
        }
        if (needle.m_Size > m_Size) return npos;

        const uint32_t lastStart = m_Size - needle.m_Size;
        for (uint32_t i = pos; i <= lastStart; ++i) {
            if (std::memcmp(m_Data + i, needle.m_Data, needle.m_Size) == 0) {
                return i;
            }
        }
        return npos;
    }

    uint32_t StringView::rfind(char c, uint32_t pos) const {
        if (m_Size == 0) return npos;
        uint32_t start = (pos >= m_Size) ? m_Size - 1 : pos;
        for (uint32_t i = start + 1; i-- > 0; ) {
            if (m_Data[i] == c) return i;
        }
        return npos;
    }

    uint32_t StringView::rfind(StringView needle, uint32_t pos) const {
        if (needle.m_Size == 0) {
            return pos < m_Size ? pos : m_Size;
        }
        if (needle.m_Size > m_Size) return npos;

        const uint32_t lastStart = m_Size - needle.m_Size;
        const uint32_t start = pos > lastStart ? lastStart : pos;
        for (uint32_t i = start + 1; i-- > 0; ) {
            if (std::memcmp(m_Data + i, needle.m_Data, needle.m_Size) == 0) {
                return i;
            }
        }
        return npos;
    }

    bool StringView::starts_with(StringView prefix) const {
        if (prefix.m_Size > m_Size) return false;
        return std::memcmp(m_Data, prefix.m_Data, prefix.m_Size) == 0;
    }

    bool StringView::starts_with(char c) const {
        return m_Size > 0 && m_Data[0] == c;
    }

    bool StringView::ends_with(StringView suffix) const {
        if (suffix.m_Size > m_Size) return false;
        return std::memcmp(m_Data + (m_Size - suffix.m_Size),
                           suffix.m_Data,
                           suffix.m_Size) == 0;
    }

    bool StringView::ends_with(char c) const {
        return m_Size > 0 && m_Data[m_Size - 1] == c;
    }

    bool StringView::contains(StringView needle) const { return find(needle) != npos; }
    bool StringView::contains(char c) const            { return find(c) != npos; }

    // ── Comparison ───────────────────────────────────────────────────
    int StringView::compare(StringView other) const {
        const uint32_t n = (m_Size < other.m_Size) ? m_Size : other.m_Size;
        if (n > 0) {
            const int r = std::memcmp(m_Data, other.m_Data, n);
            if (r != 0) return r;
        }
        if (m_Size == other.m_Size) return 0;
        return (m_Size < other.m_Size) ? -1 : 1;
    }

    bool operator==(StringView a, StringView b) {
        return a.m_Size == b.m_Size &&
               (a.m_Size == 0 || std::memcmp(a.m_Data, b.m_Data, a.m_Size) == 0);
    }

    bool operator!=(StringView a, StringView b) { return !(a == b); }
    bool operator< (StringView a, StringView b) { return a.compare(b) <  0; }
    bool operator<=(StringView a, StringView b) { return a.compare(b) <= 0; }
    bool operator> (StringView a, StringView b) { return a.compare(b) >  0; }
    bool operator>=(StringView a, StringView b) { return a.compare(b) >= 0; }
}
