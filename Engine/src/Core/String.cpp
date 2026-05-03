//
// Created by ThewyRogue99 on 31.12.2025.
//

#include <Aether/Core/String.h>

#include <Aether/Core/Assert.h>
#include <Aether/Core/Memory/BasicAllocator.h>

#include "Memory/DefaultAllocator.h"

namespace Aether::Engine {
    // ── Construction / lifetime ──────────────────────────────────────
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

    // ── Capacity ─────────────────────────────────────────────────────
    uint32_t String::size()     const { return m_size; }
    uint32_t String::length()   const { return m_size; }
    uint32_t String::capacity() const { return m_capacity; }
    bool     String::empty()    const { return m_size == 0; }

    void String::reserve(uint32_t newCap) {
        if (newCap <= m_capacity) return;
        growTo(newCap);
    }

    // ── Element access ───────────────────────────────────────────────
    const char* String::c_str() const { return m_data ? m_data : ""; }
    const char* String::data()  const { return m_data ? m_data : ""; }
    char*       String::data()        { return m_data; }

    char String::operator[](uint32_t i) const {
        AETHER_ASSERT(i < m_size);
        return m_data[i];
    }
    char& String::operator[](uint32_t i) {
        AETHER_ASSERT(i < m_size);
        return m_data[i];
    }

    char String::at(uint32_t i) const {
        AETHER_ASSERT_MSG(i < m_size, "String::at out of range");
        return m_data[i];
    }
    char& String::at(uint32_t i) {
        AETHER_ASSERT_MSG(i < m_size, "String::at out of range");
        return m_data[i];
    }

    char String::front() const {
        AETHER_ASSERT_MSG(m_size > 0, "String::front on empty string");
        return m_data[0];
    }
    char& String::front() {
        AETHER_ASSERT_MSG(m_size > 0, "String::front on empty string");
        return m_data[0];
    }

    char String::back() const {
        AETHER_ASSERT_MSG(m_size > 0, "String::back on empty string");
        return m_data[m_size - 1];
    }
    char& String::back() {
        AETHER_ASSERT_MSG(m_size > 0, "String::back on empty string");
        return m_data[m_size - 1];
    }

    // ── Iterators ────────────────────────────────────────────────────
    const char* String::begin()  const { return m_data; }
    char*       String::begin()        { return m_data; }
    const char* String::end()    const { return m_data ? m_data + m_size : nullptr; }
    char*       String::end()          { return m_data ? m_data + m_size : nullptr; }
    const char* String::cbegin() const { return m_data; }
    const char* String::cend()   const { return m_data ? m_data + m_size : nullptr; }

    StringView String::view() const { return StringView{ data(), m_size }; }

    // ── Modifiers ────────────────────────────────────────────────────
    void String::clear() {
        m_size = 0;
        if (m_data) m_data[0] = '\0';
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

    void String::pop_back() {
        AETHER_ASSERT_MSG(m_size > 0, "String::pop_back on empty string");
        --m_size;
        m_data[m_size] = '\0';
    }

    void String::resize(uint32_t newSize) {
        resize(newSize, '\0');
    }

    void String::resize(uint32_t newSize, char fill) {
        if (newSize == m_size) return;
        if (newSize < m_size) {
            m_size = newSize;
            if (m_data) m_data[m_size] = '\0';
            return;
        }
        // grow
        if (newSize > m_capacity) growTo(newSize);
        for (uint32_t i = m_size; i < newSize; ++i) m_data[i] = fill;
        m_size = newSize;
        m_data[m_size] = '\0';
    }

    void String::erase(uint32_t pos, uint32_t count) {
        AETHER_ASSERT_MSG(pos <= m_size, "String::erase pos out of range");
        const uint32_t available = m_size - pos;
        const uint32_t toRemove = count < available ? count : available;
        if (toRemove == 0) return;

        const uint32_t tailStart = pos + toRemove;
        const uint32_t tailLen   = m_size - tailStart;
        if (tailLen > 0) {
            std::memmove(m_data + pos, m_data + tailStart, tailLen);
        }
        m_size -= toRemove;
        m_data[m_size] = '\0';
    }

    String& String::operator+=(StringView sv)    { append(sv);                            return *this; }
    String& String::operator+=(char c)           { push_back(c);                          return *this; }
    String& String::operator+=(const char* cstr) { append(StringView::FromCString(cstr)); return *this; }

    // ── Substring / search ───────────────────────────────────────────
    String String::substr(uint32_t pos, uint32_t count) const {
        return String(view().substr(pos, count), m_alloc);
    }

    uint32_t String::find(char c, uint32_t pos) const                  { return view().find(c, pos); }
    uint32_t String::find(StringView needle, uint32_t pos) const       { return view().find(needle, pos); }
    uint32_t String::rfind(char c, uint32_t pos) const                 { return view().rfind(c, pos); }
    uint32_t String::rfind(StringView needle, uint32_t pos) const      { return view().rfind(needle, pos); }

    bool String::starts_with(StringView prefix) const { return view().starts_with(prefix); }
    bool String::starts_with(char c) const            { return view().starts_with(c); }
    bool String::ends_with(StringView suffix) const   { return view().ends_with(suffix); }
    bool String::ends_with(char c) const              { return view().ends_with(c); }
    bool String::contains(StringView needle) const    { return view().contains(needle); }
    bool String::contains(char c) const               { return view().contains(c); }

    // ── Comparison ───────────────────────────────────────────────────
    int String::compare(StringView other) const { return view().compare(other); }

    bool String::operator==(const String& other) const {
        if (m_size != other.m_size) return false;
        if (m_size == 0)            return true;
        return std::memcmp(m_data, other.m_data, m_size) == 0;
    }

    bool String::operator!=(const String& other) const { return !(*this == other); }

    bool String::operator<(const String& other) const {
        const uint32_t minSize = m_size < other.m_size ? m_size : other.m_size;
        if (minSize > 0) {
            const int cmp = std::memcmp(m_data, other.m_data, minSize);
            if (cmp != 0) return cmp < 0;
        }
        return m_size < other.m_size;
    }

    bool String::operator==(StringView other) const {
        if (m_size != other.size()) return false;
        if (m_size == 0)            return true;
        return std::memcmp(m_data, other.data(), m_size) == 0;
    }
    bool String::operator!=(StringView other) const { return !(*this == other); }

    bool String::operator==(const char* cstr) const { return *this == StringView::FromCString(cstr); }
    bool String::operator!=(const char* cstr) const { return !(*this == cstr); }

    // ── Internal ─────────────────────────────────────────────────────
    void String::release() {
        if (m_data && m_alloc) {
            m_alloc->deallocate(m_data);
        }
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
    }

    void String::moveFrom(String&& other) noexcept {
        m_alloc    = other.m_alloc;
        m_data     = other.m_data;
        m_size     = other.m_size;
        m_capacity = other.m_capacity;

        other.m_data     = nullptr;
        other.m_size     = 0;
        other.m_capacity = 0;
    }

    void String::growTo(uint32_t newCap) {
        AETHER_ASSERT_MSG(m_alloc, "Allocator not set");
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
