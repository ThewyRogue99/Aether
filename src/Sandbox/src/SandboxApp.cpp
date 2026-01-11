//
// Created by ThewyRogue99 on 5.01.2026.
//

#include <Aether/Core/EntryPoint.h>

#include <Aether/Log/Log.h>
#include <Aether/Log/StdColorSink.h>
#include <Aether/Core/Application.h>

#include "SandboxLayer.h"

class SandboxApp : public Aether::Engine::Application {
public:
    SandboxApp(int argc, char** argv) : Application("Aether Sandbox", argc, argv),
        m_LogSink(std::make_unique<Aether::Log::StdColorSink>()) { }
    ~SandboxApp() override = default;

    void OnInit() override {
        Application::OnInit();

        Aether::Log::SetLogger(std::make_unique<Aether::Log::Logger>("Sandbox"));

        auto& logger = Aether::Log::GetLogger();
        logger.AddSink(*m_LogSink);
        logger.SetLevel(Aether::Log::LogLevel::Trace);

        PushLayer(Aether::Engine::MakeScope<SandboxLayer>());
    }

private:
    std::unique_ptr<Aether::Log::Sink> m_LogSink;
};

namespace Aether::Engine {
    Application* CreateApplication(int argc, char** argv) {
        return new SandboxApp(argc, argv);
    }
}