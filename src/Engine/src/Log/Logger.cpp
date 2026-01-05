//
// Created by ThewyRogue99 on 5.01.2026.
//

#include <Aether/Log/Logger.h>

#include <Aether/Log/Sink.h>
#include <Aether/Core/Assert.h>

#include <spdlog/logger.h>
#include <spdlog/sinks/sink.h>

#include <vector>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <sys/stat.h>

#include "NullSink.h"

namespace Aether::Log {
    static std::unique_ptr<Sink> s_NullSink;

    static spdlog::level::level_enum ToSpd(LogLevel lvl) {
        return static_cast<spdlog::level::level_enum>(lvl);
    }

    Logger::Logger(const char* name) {
        m_Logger = std::make_unique<spdlog::logger>(name);
        m_Logger->set_level(spdlog::level::trace);

        if (!s_NullSink) {
            s_NullSink = std::make_unique<NullSink>();
        }

        // Safety: always have at least one sink
        m_Logger->sinks().push_back(s_NullSink->GetNative());
    }

    Logger::~Logger() = default;

    void Logger::AddSink(Sink& sink) {
        m_Logger->sinks().push_back(sink.GetNative());
    }

    void Logger::SetLevel(LogLevel level) {
        m_Level = level;
    }

    bool Logger::IsEnabled(LogLevel level) const {
        return static_cast<uint8_t>(level) >= static_cast<uint8_t>(m_Level);
    }

    void Logger::Log(LogLevel level, const char* fmt, ...) {
        if (!IsEnabled(level))
            return;

        va_list args;
        va_start(args, fmt);

        int needed = std::vsnprintf(nullptr, 0, fmt, args);
        va_end(args);

        if (needed <= 0)
            return;

        std::string buffer;
        buffer.resize(static_cast<size_t>(needed) + 1);

        va_start(args, fmt);
        std::vsnprintf(buffer.data(), buffer.size(), fmt, args);
        va_end(args);

        m_Logger->log(ToSpd(level), buffer.c_str());

        if (level == LogLevel::Fatal) {
            Flush();
            AETHER_ASSERT(false);
        }
    }

    void Logger::Flush() {
        m_Logger->flush();
    }
}
