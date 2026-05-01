//
// Created by ThewyRogue99 on 5.01.2026.
//

#include <Aether/Log/StdColorSink.h>

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Aether::Log {
    class StdColorSink::Impl {
    public:
        Impl() {
            Native = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            Native->set_pattern("[%T] [%^%l%$] [%n] %v");
        }

        std::shared_ptr<spdlog::sinks::sink> Native;
    };

    StdColorSink::StdColorSink() : m_Impl(Engine::MakeScope<Impl>()) { }

    StdColorSink::~StdColorSink() = default;

    std::shared_ptr<spdlog::sinks::sink> StdColorSink::GetNative() {
        return m_Impl->Native;
    }
}