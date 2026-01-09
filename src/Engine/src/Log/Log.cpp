//
// Created by ThewyRogue99 on 5.01.2026.
//

#include <memory>

#include <Aether/Log/Log.h>

namespace Aether::Log {
    static std::unique_ptr<Logger> s_Logger;
    static std::unique_ptr<Logger> s_FallbackLogger;

    void SetLogger(Engine::Scope<Logger> logger) {
        s_Logger = std::move(logger);
    }

    Logger& GetLogger() {
        Logger* logger;

        if (!s_Logger) {
            s_FallbackLogger = std::make_unique<Logger>("Undefined");
            logger = s_FallbackLogger.get();
        } else {
            logger = s_Logger.get();
        }

        return *logger;
    }
}
