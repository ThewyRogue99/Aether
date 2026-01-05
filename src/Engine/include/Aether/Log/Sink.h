//
// Created by ThewyRogue99 on 5.01.2026.
//

#pragma once

#include <memory>

#include <Aether/Core/Core.h>

namespace spdlog::sinks { class sink; }

namespace Aether::Log {
    class AETHER_API Sink {
    public:
        virtual ~Sink() = default;

        virtual std::shared_ptr<spdlog::sinks::sink> GetNative() = 0;

    protected:
        Sink() = default;
    };
}
