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

        explicit StringView(std::string_view sv);

        [[nodiscard]] bool empty() const;

        [[nodiscard]] const char* begin() const;
        [[nodiscard]] const char* end() const;

        char operator[](uint32_t i) const;

        [[nodiscard]] int compare(StringView other) const;

        friend bool operator==(StringView a, StringView b);
        friend bool operator!=(StringView a, StringView b);

        [[nodiscard]] const char* data() const;
        [[nodiscard]] uint32_t size() const;

    private:
        const char* m_Data = nullptr;
        uint32_t m_Size = 0;
    };
}
