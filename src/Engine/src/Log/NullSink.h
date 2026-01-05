//
// Created by ThewyRogue99 on 5.01.2026.
//

#pragma once

#include <Aether/Log/Sink.h>

namespace Aether::Log {
    class NullSink : public Sink {
    public:
        NullSink();
        ~NullSink() override;

        std::shared_ptr<spdlog::sinks::sink> GetNative() override;

    private:
        class Impl;
        std::shared_ptr<Impl> m_Impl;
    };
}
