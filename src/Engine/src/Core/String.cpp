//
// Created by ThewyRogue99 on 31.12.2025.
//

#include <Aether/Core/String.h>

#include <Aether/Core/Assert.h>
#include <Aether/Core/Memory/BasicAllocator.h>

#include "Memory/DefaultAllocator.h"

namespace Aether::Core {
    String::String() = default;

    String::String(const String& other)
        : m_alloc(other.m_alloc ? other.m_alloc : &DefaultAllocator()) {
        assign(other.view());
    }

    String::String(String&& other) noexcept {
        moveFrom(std::move(other));
    }

    String::String(StringView sv, BasicAllocator* alloc)
        : m_alloc(alloc ? alloc : &DefaultAllocator()) {
        assign(sv);
    }

    String::String(const char* cstr, BasicAllocator* alloc)
        : m_alloc(alloc ? alloc : &DefaultAllocator()) {
        assign(StringView::FromCString(cstr));
    }

    String::~String() { release(); }

    String& String::operator=(const String& other) {
        if (this == &other) return *this;
        if (!m_alloc) m_alloc = other.m_alloc ? other.m_alloc : &DefaultAllocator();
        assign(other.view());
        return *this;
    }

    String& String::operator=(String&& other) noexcept {
        if (this == &other) return *this;
        release();
        moveFrom(std::move(other));
        return *this;
    }

    uint32_t String::size() const { return m_size; }
    uint32_t String::capacity() const { return m_capacity; }
    bool String::empty() const { return m_size == 0; }

    const char* String::c_str() const { return m_data ? m_data : ""; }
    const char* String::data()  const { return m_data ? m_data : ""; }
    char* String::data() { return m_data; }

    StringView String::view() const { return StringView{ data(), m_size }; }

    void String::clear() {
        m_size = 0;
        if (m_data) m_data[0] = '\0';
    }

    void String::reserve(uint32_t newCap) {
        if (newCap <= m_capacity) return;
        growTo(newCap);
    }

    void String::assign(StringView sv) {
        clear();
        reserve(sv.size());
        if (!sv.empty()) {
            std::memcpy(m_data, sv.data(), sv.size());
            m_size = sv.size();
            m_data[m_size] = '\0';
        }
    }

    void String::append(StringView sv) {
        if (sv.empty()) return;
        const uint32_t required = m_size + sv.size();
        if (required > m_capacity) {
            uint32_t newCap = m_capacity ? (m_capacity + m_capacity / 2) : 16;
            if (newCap < required) newCap = required;
            growTo(newCap);
        }
        std::memcpy(m_data + m_size, sv.data(), sv.size());
        m_size += sv.size();
        m_data[m_size] = '\0';
    }

    void String::push_back(char c) {
        if (m_size + 1 > m_capacity) {
            uint32_t newCap = m_capacity ? (m_capacity + m_capacity / 2) : 16;
            if (newCap < m_size + 1) newCap = m_size + 1;
            growTo(newCap);
        }
        m_data[m_size++] = c;
        m_data[m_size] = '\0';
    }

    void String::release() {
        if (m_data && m_alloc) {
            m_alloc->deallocate(m_data);
        }
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
    }

    void String::moveFrom(String&& other) noexcept {
        m_alloc = other.m_alloc;
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    void String::growTo(uint32_t newCap) {
        AETHER_ASSERT(m_alloc && "Allocator not set");
        char* newData = static_cast<char*>(m_alloc->allocate(static_cast<std::size_t>(newCap) + 1));
        AETHER_ASSERT(newData);

        if (m_data && m_size) {
            std::memcpy(newData, m_data, m_size);
        }
        newData[m_size] = '\0';

        if (m_data) m_alloc->deallocate(m_data);
        m_data = newData;
        m_capacity = newCap;
    }
}
