//
// Created by ThewyRogue99 on 1.01.2026.
//

#pragma once

#include <vector>

#include "StringView.h"

namespace Aether::Engine {
    class AETHER_API StringBuilder {
    public:
        StringBuilder();

        void clear();

        [[nodiscard]] uint32_t size() const;
        [[nodiscard]] bool empty() const;

        void append(StringView sv);

        void append(const char* cstr);

        void append(char c);

        void append_u32(uint32_t v);

        StringView finish();

    private:
        std::vector<StringView> m_parts;
        uint32_t m_size = 0;
    };
}
