//
// Created by ThewyRogue99 on 5.01.2026.
//

#pragma once

#include <Aether/Core/Application.h>

namespace Aether::Log {
    class Sink;
}

class SandboxApp : public Aether::Engine::Application {
public:
    SandboxApp(int argc, char** argv);
    ~SandboxApp() override;

    void OnInit() override;

private:
    std::unique_ptr<Aether::Log::Sink> m_LogSink;
};
