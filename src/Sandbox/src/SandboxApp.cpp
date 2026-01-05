//
// Created by ThewyRogue99 on 5.01.2026.
//

#include "SandboxApp.h"

#include <Aether/Log/Log.h>
#include <Aether/Log/StdColorSink.h>

SandboxApp::SandboxApp(int argc, char** argv)
    : Application("Aether Sandbox", argc, argv), m_LogSink(std::make_unique<Aether::Log::StdColorSink>()) { }

SandboxApp::~SandboxApp() = default;

void SandboxApp::OnInit() {
    Application::OnInit();

    Aether::Log::SetLogger(std::make_unique<Aether::Log::Logger>("Sandbox"));

    auto& logger = Aether::Log::GetLogger();
    logger.AddSink(*m_LogSink);
    logger.SetLevel(Aether::Log::LogLevel::Trace);
}
