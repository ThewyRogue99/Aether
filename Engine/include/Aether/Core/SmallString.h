//
// Created by ThewyRogue99 on 31.12.2025.
//

#pragma once

#include "Assert.h"
#include "StringView.h"

namespace Aether::Engine {
    enum class AETHER_API SmallStringOverflow : uint8_t {
        Assert,
        Truncate,
    };

    template<uint32_t N = 64, SmallStringOverflow OverflowPolicy = SmallStringOverflow::Assert>
    class AETHER_API SmallString {
    public:
        static_assert(N >= 2, "Need space for at least 1 char + null terminator.");

        SmallString() { m_buf[0] = '\0'; }

        explicit SmallString(StringView sv) { assign(sv); }
        explicit SmallString(const char* cstr) { assign(StringView::FromCString(cstr)); }

        uint32_t size() const { return m_size; }
        uint32_t capacity() const { return N - 1; } // excluding null terminator
        bool empty() const { return m_size == 0; }

        const char* c_str() const { return m_buf; }
        char* data()  { return m_buf; }
        const char* data() const { return m_buf; }

        StringView view() const { return StringView{ m_buf, m_size }; }

        void clear() {
            m_size = 0;
            m_buf[0] = '\0';
        }

        void assign(StringView sv) {
            clear();
            append(sv);
        }

        void append(StringView sv) {
            if (sv.size() == 0) return;

            const uint32_t cap = capacity();
            if (m_size + sv.size > cap) {
                if constexpr (OverflowPolicy == SmallStringOverflow::Assert) {
                    AETHER_ASSERT_MSG(false, "SmallString overflow");
                } else {
                    const uint32_t remaining = (m_size < cap) ? (cap - m_size) : 0;
                    sv.size = remaining;
                }
            }

            if (sv.size() > 0) {
                std::memcpy(m_buf + m_size, sv.data(), sv.size());
                m_size += sv.size;
                m_buf[m_size] = '\0';
            }
        }

        void push_back(char c) {
            const uint32_t cap = capacity();
            if (m_size + 1 > cap) {
                if constexpr (OverflowPolicy == SmallStringOverflow::Assert) {
                    AETHER_ASSERT_MSG(false, "SmallString overflow");
                    return;
                } else {
                    return; // truncate: ignore
                }
            }
            m_buf[m_size++] = c;
            m_buf[m_size] = '\0';
        }

        friend SmallString operator+(SmallString a, StringView b) {
            a.append(b);
            return a;
        }

    private:
        char m_buf[N]{};
        uint32_t m_size = 0;
    };
}
