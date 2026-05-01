//
// Created by ThewyRogue99 on 1.01.2026.
//

#pragma once

#include <Aether/Core/Memory/Pointer.h>

namespace Aether::Engine {
    class TempScope {
    public:
        TempScope();
        ~TempScope();

        TempScope(const TempScope&) = delete;
        TempScope& operator=(const TempScope&) = delete;

    private:
        class Impl;
        Scope<Impl> m_impl;
    };
}
