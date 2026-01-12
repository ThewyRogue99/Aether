//
// Created by ThewyRogue99 on 31.12.2025.
//

#pragma once

#include <Aether/Core/String.h>

#include <Aether/Core/Memory/Pointer.h>

namespace Aether::Engine {
    class StringView;

    class AETHER_API Name {
    public:
        Name();

        explicit Name(StringView& sv);
        explicit Name(const char* cstr);

        [[nodiscard]] uint32_t id() const;
        explicit operator bool() const;

        friend bool operator==(const Name& a, const Name& b);
        friend bool operator!=(const Name& a, const Name& b);

        // Debug/Editor only: get original string (thread-safe)
        static String ToString(const Name& n);

    private:
        class Impl;
        Scope<Impl> m_impl;
    };
}
