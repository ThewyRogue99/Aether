//
// Created by ThewyRogue99 on 31.12.2025.
//

#pragma once

#include "Core.h"

#include <string_view>

namespace Aether::Core {
    class AETHER_API StringView {
    public:
        StringView();
        StringView(const char* d, uint32_t n);

        static StringView FromCString(const char* cstr);

        constexpr explicit StringView(std::string_view sv);

        [[nodiscard]] constexpr bool empty() const;

        [[nodiscard]] constexpr const char* begin() const;
        [[nodiscard]] constexpr const char* end() const;

        constexpr char operator[](uint32_t i) const;

        [[nodiscard]] int compare(StringView other) const;

        friend bool operator==(StringView a, StringView b);
        friend bool operator!=(StringView a, StringView b);

        const char* data = nullptr;
        uint32_t size = 0;
    };
}
