//
// Created by ThewyRogue99 on 31.12.2025.
//

#pragma once

#include "Core.h"
#include "StringView.h"

namespace Aether::Engine {
    class BasicAllocator;

    class AETHER_API String {
    public:
        String();
        String(const String& other);
        String(String&& other) noexcept;
        String(const char* cstr, BasicAllocator* alloc = nullptr);
        explicit String(StringView sv, BasicAllocator* alloc = nullptr);

        ~String();

        String& operator=(const String& other);
        String& operator=(String&& other) noexcept;

        [[nodiscard]] uint32_t size() const;
        [[nodiscard]] uint32_t capacity() const; // excluding null terminator
        [[nodiscard]] bool empty() const;

        [[nodiscard]] const char* c_str() const;
        [[nodiscard]] const char* data() const;
        char* data();

        [[nodiscard]] StringView view() const;

        void clear();

        void reserve(uint32_t newCap);

        void assign(StringView sv);

        void append(StringView sv);

        void push_back(char c);

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
