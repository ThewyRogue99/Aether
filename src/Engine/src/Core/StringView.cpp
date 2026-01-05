//
// Created by ThewyRogue99 on 31.12.2025.
//

#include <Aether/Core/StringView.h>

#include <Aether/Core/Assert.h>

namespace Aether::Core {
    StringView::StringView() = default;
    StringView::StringView(const char* d, uint32_t n) : m_Data(d), m_Size(n) { }

    StringView StringView::FromCString(const char* cstr) {
        if (!cstr) return {};
        return StringView{ cstr, static_cast<uint32_t>(std::strlen(cstr)) };
    }

    StringView::StringView(std::string_view sv)
        : m_Data(sv.data()), m_Size(static_cast<uint32_t>(sv.size())) {}

    bool StringView::empty() const { return m_Size == 0; }

    const char* StringView::begin() const { return m_Data; }
    const char* StringView::end()   const { return m_Data + m_Size; }

    char StringView::operator[](uint32_t i) const {
        AETHER_ASSERT(i < size());
        return m_Data[i];
    }

    int StringView::compare(StringView other) const {
        const uint32_t n = (m_Size < other.m_Size) ? m_Size : other.m_Size;
        if (n > 0) {
            const int r = std::memcmp(m_Data, other.m_Data, n);
            if (r != 0) return r;
        }
        if (m_Size == other.m_Size) return 0;
        return (m_Size < other.m_Size) ? -1 : 1;
    }

    const char* StringView::data() const {
        return m_Data;
    }

    uint32_t StringView::size() const {
        return m_Size;
    }

    bool operator==(StringView a, StringView b) {
        return a.m_Size == b.m_Size && (a.m_Size == 0 || std::memcmp(a.m_Data, b.m_Data, a.m_Size) == 0);
    }

    bool operator!=(StringView a, StringView b) { return !(a == b); }
}