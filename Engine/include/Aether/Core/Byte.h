//
// Created by ThewyRogue99 on 20.05.2026.
//

#pragma once

#include <type_traits>

namespace Aether::Engine {
    enum class Byte : unsigned char {};
  
    inline constexpr Byte operator|(Byte __lhs, Byte __rhs) noexcept {
      return static_cast<Byte>(
          static_cast<unsigned char>(static_cast<unsigned int>(__lhs) | static_cast<unsigned int>(__rhs)));
    }

    inline constexpr Byte& operator|=(Byte& __lhs, Byte __rhs) noexcept {
      return __lhs = __lhs | __rhs;
    }

    inline constexpr Byte operator&(Byte __lhs, Byte __rhs) noexcept {
      return static_cast<Byte>(
          static_cast<unsigned char>(static_cast<unsigned int>(__lhs) & static_cast<unsigned int>(__rhs)));
    }

    inline constexpr Byte& operator&=(Byte& __lhs, Byte __rhs) noexcept {
      return __lhs = __lhs & __rhs;
    }

    inline constexpr Byte operator^(Byte __lhs, Byte __rhs) noexcept {
      return static_cast<Byte>(
          static_cast<unsigned char>(static_cast<unsigned int>(__lhs) ^ static_cast<unsigned int>(__rhs)));
    }

    inline constexpr Byte& operator^=(Byte& __lhs, Byte __rhs) noexcept {
      return __lhs = __lhs ^ __rhs;
    }

    inline constexpr Byte operator~(Byte __b) noexcept {
      return static_cast<Byte>(static_cast<unsigned char>(~static_cast<unsigned int>(__b)));
    }

    template <class _Integer, std::enable_if_t<std::is_integral_v<_Integer>, int> = 0>
    constexpr Byte& operator<<=(Byte& __lhs, _Integer __shift) noexcept {
      return __lhs = __lhs << __shift;
    }

    template <class _Integer, std::enable_if_t<std::is_integral_v<_Integer>, int> = 0>
    constexpr Byte operator<<(Byte __lhs, _Integer __shift) noexcept {
      return static_cast<Byte>(static_cast<unsigned char>(static_cast<unsigned int>(__lhs) << __shift));
    }

    template <class _Integer, std::enable_if_t<std::is_integral_v<_Integer>, int> = 0>
    constexpr Byte& operator>>=(Byte& __lhs, _Integer __shift) noexcept {
      return __lhs = __lhs >> __shift;
    }

    template <class _Integer, std::enable_if_t<std::is_integral_v<_Integer>, int> = 0>
    constexpr Byte operator>>(Byte __lhs, _Integer __shift) noexcept {
      return static_cast<Byte>(static_cast<unsigned char>(static_cast<unsigned int>(__lhs) >> __shift));
    }

    template <class _Integer, std::enable_if_t<std::is_integral_v<_Integer>, int> = 0>
    [[nodiscard]] constexpr _Integer to_integer(Byte __b) noexcept {
      return static_cast<_Integer>(__b);
    }
}

