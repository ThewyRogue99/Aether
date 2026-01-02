//
// Created by ThewyRogue99 on 31.12.2025.
//

#include <Aether/Core/StringView.h>

#include <Aether/Core/Assert.h>

namespace Aether::Core {
    constexpr StringView::StringView() = default;
    constexpr StringView::StringView(const char* d, uint32_t n) : data(d), size(n) { }

    StringView StringView::FromCString(const char* cstr) {
        if (!cstr) return {};
        return StringView{ cstr, static_cast<uint32_t>(std::strlen(cstr)) };
    }

    constexpr StringView::StringView(std::string_view sv)
        : data(sv.data()), size(static_cast<uint32_t>(sv.size())) {}

    constexpr bool StringView::empty() const { return size == 0; }

    constexpr const char* StringView::begin() const { return data; }
    constexpr const char* StringView::end()   const { return data + size; }

    constexpr char StringView::operator[](uint32_t i) const {
        AETHER_ASSERT(i < size);
        return data[i];
    }

    int StringView::compare(StringView other) const {
        const uint32_t n = (size < other.size) ? size : other.size;
        if (n > 0) {
            const int r = std::memcmp(data, other.data, n);
            if (r != 0) return r;
        }
        if (size == other.size) return 0;
        return (size < other.size) ? -1 : 1;
    }

    bool operator==(StringView a, StringView b) {
        return a.size == b.size && (a.size == 0 || std::memcmp(a.data, b.data, a.size) == 0);
    }
    bool operator!=(StringView a, StringView b) { return !(a == b); }
}