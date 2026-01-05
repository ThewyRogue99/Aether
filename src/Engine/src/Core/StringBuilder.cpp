//
// Created by ThewyRogue99 on 1.01.2026.
//

#include <Aether/Core/StringBuilder.h>

#include <Memory/TempAllocator.h>
#include <Aether/Core/Memory/LinearAllocator.h>

namespace Aether::Core {
    StringBuilder::StringBuilder() = default;

    void StringBuilder::clear() {
        m_parts.clear();
        m_size = 0;
    }

    uint32_t StringBuilder::size() const { return m_size; }
    bool StringBuilder::empty() const { return m_size == 0; }

    void StringBuilder::append(StringView sv) {
        if (sv.size() == 0) return;
        m_parts.push_back(sv);
        m_size += sv.size();
    }

    void StringBuilder::append(const char* cstr) {
        append(StringView::FromCString(cstr));
    }

    void StringBuilder::append(char c) {
        char* p = static_cast<char*>(TempAllocator().allocate(1, alignof(char)));
        *p = c;
        append(StringView{ p, 1 });
    }

    void StringBuilder::append_u32(uint32_t v) {
        char tmp[16];
        char* p = tmp + sizeof(tmp);
        do {
            *--p = char('0' + (v % 10));
            v /= 10;
        } while (v);

        append(StringView{ p, uint32_t(tmp + sizeof(tmp) - p) });
    }

    StringView StringBuilder::finish() {
        if (m_size == 0)
            return {};

        char* out = static_cast<char*>(TempAllocator().allocate(m_size + 1, alignof(char)));

        char* dst = out;
        for (const StringView& sv : m_parts) {
            std::memcpy(dst, sv.data(), sv.size());
            dst += sv.size();
        }

        out[m_size] = '\0';
        return StringView{ out, m_size };
    }
}
