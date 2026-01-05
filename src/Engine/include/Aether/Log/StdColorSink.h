//
// Created by ThewyRogue99 on 5.01.2026.
//

#pragma once

#include <Aether/Log/Sink.h>
#include <Aether/Core/Core.h>

namespace Aether::Log {
    class AETHER_API StdColorSink : public Sink {
    public:
        StdColorSink();
        ~StdColorSink() override;

        std::shared_ptr<spdlog::sinks::sink> GetNative() override;

    private:
        class Impl;
        std::unique_ptr<Impl> m_Impl;
    };
}
