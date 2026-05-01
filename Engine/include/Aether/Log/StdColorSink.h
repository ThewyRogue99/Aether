//
// Created by ThewyRogue99 on 5.01.2026.
//

#pragma once

#include <Aether/Log/Sink.h>
#include <Aether/Core/Core.h>
#include <Aether/Core/Memory/Pointer.h>

namespace Aether::Log {
    class AETHER_API StdColorSink : public Sink {
    public:
        StdColorSink();
        ~StdColorSink() override;

        std::shared_ptr<spdlog::sinks::sink> GetNative() override;

    private:
        class Impl;
        Engine::Scope<Impl> m_Impl;
    };
}
