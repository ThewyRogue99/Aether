//
// Created by ThewyRogue99 on 15.05.2026.
//

#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <utility>

#include <Aether/Core/Assert.h>
#include <Aether/Core/BasicStringView.h>
#include <Aether/Core/Memory/BasicAllocator.h>

namespace Aether::Engine {
    template<typename CharT>
    class BasicString {
    public:
        using CharType = CharT;
        using Traits = std::char_traits<CharT>;
        using ViewType = BasicStringView<CharT>;

        static constexpr uint32_t npos = ViewType::npos;

        // ── Construction / lifetime ───────────────────────────────────────────────

        BasicString();
        BasicString(const BasicString& other);
        BasicString(BasicString&& other) noexcept;

        BasicString(const CharT* cstr, BasicAllocator* alloc = nullptr);
        explicit BasicString(ViewType sv, BasicAllocator* alloc = nullptr);

        ~BasicString();

        BasicString& operator=(const BasicString& other);
        BasicString& operator=(BasicString&& other) noexcept;

        // ── Capacity ──────────────────────────────────────────────────────────────

        [[nodiscard]] uint32_t size() const;
        [[nodiscard]] uint32_t length() const;
        [[nodiscard]] uint32_t capacity() const;
        [[nodiscard]] bool empty() const;

        void reserve(uint32_t newCap);

        // ── Element access ────────────────────────────────────────────────────────

        [[nodiscard]] const CharT* c_str() const;
        [[nodiscard]] const CharT* data() const;
        CharT* data();

        [[nodiscard]] CharT operator[](uint32_t i) const;
        CharT& operator[](uint32_t i);
        [[nodiscard]] CharT at(uint32_t i) const;
        CharT& at(uint32_t i);
        [[nodiscard]] CharT front() const;
        CharT& front();
        [[nodiscard]] CharT back() const;
        CharT& back();

        // ── Iterators ─────────────────────────────────────────────────────────────

        [[nodiscard]] const CharT* begin() const;
        CharT* begin();
        [[nodiscard]] const CharT* end() const;
        CharT* end();
        [[nodiscard]] const CharT* cbegin() const;
        [[nodiscard]] const CharT* cend() const;

        // ── Conversion ────────────────────────────────────────────────────────────

        [[nodiscard]] ViewType view() const;

        operator ViewType() const { return view(); }  // NOLINT(google-explicit-constructor)

        operator std::basic_string_view<CharT>() const {  // NOLINT(google-explicit-constructor)
            return std::basic_string_view<CharT>(data(), size());
        }

        // ── Modifiers ─────────────────────────────────────────────────────────────

        void clear();

        void assign(ViewType sv);

        void append(ViewType sv);
        void push_back(CharT c);
        void pop_back();

        void resize(uint32_t newSize);
        void resize(uint32_t newSize, CharT fill);

        void erase(uint32_t pos = 0, uint32_t count = npos);

        BasicString& operator+=(ViewType sv);
        BasicString& operator+=(CharT c);
        BasicString& operator+=(const CharT* cstr);

        // ── Substring / search ────────────────────────────────────────────────────

        [[nodiscard]] BasicString substr(uint32_t pos = 0, uint32_t count = npos) const;

        [[nodiscard]] uint32_t find(CharT c, uint32_t pos = 0) const;
        [[nodiscard]] uint32_t find(ViewType needle, uint32_t pos = 0) const;
        [[nodiscard]] uint32_t rfind(CharT c, uint32_t pos = npos) const;
        [[nodiscard]] uint32_t rfind(ViewType needle, uint32_t pos = npos) const;

        [[nodiscard]] bool starts_with(ViewType prefix) const;
        [[nodiscard]] bool starts_with(CharT c) const;
        [[nodiscard]] bool ends_with(ViewType suffix) const;
        [[nodiscard]] bool ends_with(CharT c) const;
        [[nodiscard]] bool contains(ViewType needle) const;
        [[nodiscard]] bool contains(CharT c) const;

        // ── Comparison ────────────────────────────────────────────────────────────

        [[nodiscard]] int compare(ViewType other) const;

        bool operator==(const BasicString& other) const;
        bool operator!=(const BasicString& other) const;
        bool operator< (const BasicString& other) const;

        bool operator==(ViewType other) const;
        bool operator!=(ViewType other) const;

        bool operator==(const CharT* cstr) const;
        bool operator!=(const CharT* cstr) const;

    private:
        void release();
        void moveFrom(BasicString&& other) noexcept;
        void growTo(uint32_t newCap);

        BasicAllocator* m_alloc = nullptr;
        CharT* m_data = nullptr;
        uint32_t m_size = 0;
        uint32_t m_capacity = 0;
    };

    // ── Free operator+ ────────────────────────────────────────────────────────────

    template<typename CharT>
    BasicString<CharT> operator+(BasicString<CharT> a, const BasicString<CharT>& b) {
        a.append(b.view());
        return a;
    }

    template<typename CharT>
    BasicString<CharT> operator+(BasicString<CharT> a, BasicStringView<CharT> b) {
        a.append(b);
        return a;
    }

    template<typename CharT>
    BasicString<CharT> operator+(BasicString<CharT> a, const CharT* b) {
        a.append(BasicStringView<CharT>(b));
        return a;
    }

    template<typename CharT>
    BasicString<CharT> operator+(BasicString<CharT> a, CharT c) {
        a.push_back(c);
        return a;
    }
} // namespace Aether::Engine

// ── std::hash specialisation (covers all BasicString<CharT>) ─────────────────
template<typename CharT>
struct std::hash<Aether::Engine::BasicString<CharT>> {
    std::size_t operator()(const Aether::Engine::BasicString<CharT>& s) const noexcept {
        return std::hash<std::basic_string_view<CharT>>{}(
            static_cast<std::basic_string_view<CharT>>(s)
        );
    }
};

// ── Inline definitions ────────────────────────────────────────────────────────
#include <Aether/Core/BasicString.inl>
