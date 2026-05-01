//
// Created by ThewyRogue99 on 5.01.2026.
//

#include <Aether/Log/Log.h>

#include <memory>

namespace Aether::Log {
    static Engine::Scope<Logger> s_Logger;
    static Engine::Scope<Logger> s_FallbackLogger;

    void SetLogger(Engine::Scope<Logger> logger) {
        s_Logger = std::move(logger);
    }

    Logger& GetLogger() {
        Logger* logger;

        if (!s_Logger) {
            s_FallbackLogger = Engine::MakeScope<Logger>("Undefined");
            logger = s_FallbackLogger.get();
        } else {
            logger = s_Logger.get();
        }

        return *logger;
    }
}
