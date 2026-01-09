//
// Created by ThewyRogue99 on 1.01.2026.
//

#pragma once

#include <memory>

namespace Aether::Engine {
    class TempScope {
    public:
        TempScope();
        ~TempScope();

        TempScope(const TempScope&) = delete;
        TempScope& operator=(const TempScope&) = delete;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
