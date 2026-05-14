//
// Created by ThewyRogue99 on 14.05.2026.
//

#pragma once

#include <Aether/Core/Assert.h>

#include <utility>
#include <variant>

namespace Aether::Engine {
    template<typename E>
    class Unexpected {
    public:
        explicit constexpr Unexpected(const E& e) : m_Error(e) {}
        explicit constexpr Unexpected(E&& e) : m_Error(std::move(e)) {}

        [[nodiscard]] constexpr E& Error() & noexcept { return m_Error; }
        [[nodiscard]] constexpr const E& Error() const& noexcept { return m_Error; }
        [[nodiscard]] constexpr E Error() && noexcept { return std::move(m_Error); }

    private:
        E m_Error;
    };

    // Deduction guide: Unexpected("msg") → Unexpected<const char*>
    template<typename E>
    Unexpected(E) -> Unexpected<E>;

    template<typename T, typename E>
    class Expected {
    public:
        constexpr Expected()
            requires std::default_initializable<T>
            : m_Storage(std::in_place_index<kValue>) {}

        constexpr Expected(const T& v) : m_Storage(std::in_place_index<kValue>, v) {}
        constexpr Expected(T&& v) : m_Storage(std::in_place_index<kValue>, std::move(v)) {}

        template<typename G>
        constexpr Expected(Unexpected<G> u)
            : m_Storage(std::in_place_index<kError>, std::move(u).Error()) {}

        Expected(const Expected&) = default;
        Expected(Expected&&) = default;
        Expected& operator=(const Expected&) = default;
        Expected& operator=(Expected&&) = default;

        // ── Observers ─────────────────────────────────────────────────────────────

        [[nodiscard]] constexpr bool has_value() const noexcept {
            return m_Storage.index() == kValue;
        }

        [[nodiscard]] constexpr explicit operator bool() const noexcept {
            return has_value();
        }

        // ── Value access (asserts in debug if in error state) ─────────────────────

        [[nodiscard]] constexpr T& value() & {
            AETHER_ASSERT_MSG(has_value(), "Expected::value() called on an error state");
            return std::get<kValue>(m_Storage);
        }
        [[nodiscard]] constexpr const T& value() const& {
            AETHER_ASSERT_MSG(has_value(), "Expected::value() called on an error state");
            return std::get<kValue>(m_Storage);
        }
        [[nodiscard]] constexpr T value() && {
            AETHER_ASSERT_MSG(has_value(), "Expected::value() called on an error state");
            return std::get<kValue>(std::move(m_Storage));
        }

        [[nodiscard]] constexpr T& operator*() & { return value(); }
        [[nodiscard]] constexpr const T& operator*() const& { return value(); }
        [[nodiscard]] constexpr T operator*() && { return std::move(*this).value(); }

        [[nodiscard]] constexpr T* operator->() { AETHER_ASSERT(has_value()); return &std::get<kValue>(m_Storage); }
        [[nodiscard]] constexpr const T* operator->() const { AETHER_ASSERT(has_value()); return &std::get<kValue>(m_Storage); }

        // Returns the value if present, otherwise returns (and converts) the fallback.
        template<typename U>
        [[nodiscard]] constexpr T value_or(U&& fallback) const& {
            return has_value() ? value() : static_cast<T>(std::forward<U>(fallback));
        }
        template<typename U>
        [[nodiscard]] constexpr T value_or(U&& fallback) && {
            return has_value() ? std::move(*this).value() : static_cast<T>(std::forward<U>(fallback));
        }

        // ── Error access (asserts in debug if in value state) ─────────────────────

        [[nodiscard]] constexpr E& error() & {
            AETHER_ASSERT_MSG(!has_value(), "Expected::error() called on a value state");
            return std::get<kError>(m_Storage);
        }
        [[nodiscard]] constexpr const E& error() const& {
            AETHER_ASSERT_MSG(!has_value(), "Expected::error() called on a value state");
            return std::get<kError>(m_Storage);
        }
        [[nodiscard]] constexpr E error() && {
            AETHER_ASSERT_MSG(!has_value(), "Expected::error() called on a value state");
            return std::get<kError>(std::move(m_Storage));
        }

    private:
        static constexpr std::size_t kValue = 0;
        static constexpr std::size_t kError = 1;

        std::variant<T, E> m_Storage;
    };
} // namespace Aether::Engine
