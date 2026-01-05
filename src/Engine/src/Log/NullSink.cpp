//
// Created by ThewyRogue99 on 5.01.2026.
//

#include "NullSink.h"

#include <spdlog/sinks/sink.h>

namespace Aether::Log {
    class NullSink::Impl : public spdlog::sinks::sink {
    public:
        Impl() = default;

        void log(const spdlog::details::log_msg&) override { }
        void flush() override { }

        void set_pattern(const std::string&) override { }
        void set_formatter(std::unique_ptr<spdlog::formatter>) override { }
    };

    NullSink::NullSink() : m_Impl(std::make_shared<Impl>()) { }

    NullSink::~NullSink() = default;

    std::shared_ptr<spdlog::sinks::sink> NullSink::GetNative() {
        return m_Impl;
    }
}
