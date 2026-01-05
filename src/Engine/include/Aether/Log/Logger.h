//
// Created by ThewyRogue99 on 5.01.2026.
//

#pragma once

#include <memory>

#include <Aether/Log/LogLevel.h>

namespace spdlog {
    class logger;
}

namespace Aether::Log {
    class Sink;

    class AETHER_API Logger {
    public:
        explicit Logger(const char* name);
        ~Logger();

        void AddSink(Sink& sink);
        void SetLevel(LogLevel level);
        [[nodiscard]] bool IsEnabled(LogLevel level) const;
        void Flush();

        void Log(LogLevel level, const char* fmt, ...);

    private:
        std::unique_ptr<spdlog::logger> m_Logger;
        LogLevel m_Level = LogLevel::Info;
    };
}
