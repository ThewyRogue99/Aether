//
// Created by ThewyRogue99 on 31.12.2025.
//

#pragma once

#include "Core.h"
#include "StringView.h"
#include <cstring>
#include <string_view>

namespace Aether::Engine {
    class BasicAllocator;

    class AETHER_API String {
    public:
        // Sentinel "not found" value, matches std::string::npos semantics.
        static constexpr uint32_t npos = StringView::npos;

        // ── Construction / lifetime ──────────────────────────────────
        String();
        String(const String& other);
        String(String&& other) noexcept;
        String(const char* cstr, BasicAllocator* alloc = nullptr);
        explicit String(StringView sv, BasicAllocator* alloc = nullptr);

        ~String();

        String& operator=(const String& other);
        String& operator=(String&& other) noexcept;

        // ── Capacity ─────────────────────────────────────────────────
        [[nodiscard]] uint32_t size() const;
        [[nodiscard]] uint32_t length() const;     // alias for size()
        [[nodiscard]] uint32_t capacity() const;   // excluding null terminator
        [[nodiscard]] bool empty() const;

        void reserve(uint32_t newCap);

        // ── Element access ───────────────────────────────────────────
        [[nodiscard]] const char* c_str() const;
        [[nodiscard]] const char* data() const;
        char* data();

        [[nodiscard]] char  operator[](uint32_t i) const;
        char& operator[](uint32_t i);

        [[nodiscard]] char  at(uint32_t i) const;  // bounds-checked
        char& at(uint32_t i);                      // bounds-checked

        [[nodiscard]] char  front() const;
        char& front();
        [[nodiscard]] char  back() const;
        char& back();

        // ── Iterators ────────────────────────────────────────────────
        [[nodiscard]] const char* begin() const;
        char* begin();
        [[nodiscard]] const char* end() const;
        char* end();
        [[nodiscard]] const char* cbegin() const;
        [[nodiscard]] const char* cend() const;

        // ── Conversion ───────────────────────────────────────────────
        [[nodiscard]] StringView view() const;
        operator StringView() const { return view(); }
        operator std::string_view() const { return std::string_view(data(), size()); }

        // ── Modifiers ────────────────────────────────────────────────
        void clear();

        void assign(StringView sv);

        void append(StringView sv);
        void push_back(char c);
        void pop_back();

        void resize(uint32_t newSize);
        void resize(uint32_t newSize, char fill);

        // Erase `count` chars starting at `pos`. count == npos erases to the end.
        void erase(uint32_t pos = 0, uint32_t count = npos);

        String& operator+=(StringView sv);
        String& operator+=(char c);
        String& operator+=(const char* cstr);

        // ── Substring / search (delegates to view()) ─────────────────
        [[nodiscard]] String substr(uint32_t pos = 0, uint32_t count = npos) const;

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

        bool operator==(const String& other) const;
        bool operator!=(const String& other) const;
        bool operator< (const String& other) const;

        bool operator==(StringView other) const;
        bool operator!=(StringView other) const;

        bool operator==(const char* cstr) const;
        bool operator!=(const char* cstr) const;

    private:
        void release();
        void moveFrom(String&& other) noexcept;
        void growTo(uint32_t newCap);

    private:
        BasicAllocator* m_alloc = nullptr;
        char* m_data = nullptr;
        uint32_t m_size = 0;
        uint32_t m_capacity = 0;
    };

    AETHER_API inline String operator+(String a, StringView b) {
        a.append(b);
        return a;
    }
}
