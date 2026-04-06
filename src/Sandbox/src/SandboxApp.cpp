//
// Created by ThewyRogue99 on 5.01.2026.
//

#include <Aether/Core/EntryPoint.h>

#include <Aether/Log/Log.h>
#include <Aether/Log/StdColorSink.h>
#include <Aether/Core/Application.h>

#include "SandboxLayer.h"
#include "Aether/Math/Math.h"
#include "Aether/Renderer/Buffer.h"
#include "Aether/Renderer/Renderer.h"

namespace Aether {
    class SandboxApp : public Aether::Engine::Application {
    public:
        SandboxApp(int argc, char** argv) : Application("Aether Sandbox", argc, argv),
            m_LogSink(std::make_unique<Aether::Log::StdColorSink>()) { }
        ~SandboxApp() override = default;

        void OnInit() override {
            Aether::Log::SetLogger(std::make_unique<Aether::Log::Logger>("Sandbox"));

            auto& logger = Aether::Log::GetLogger();
            logger.AddSink(*m_LogSink);
            logger.SetLevel(Aether::Log::LogLevel::Trace);

            PushLayer(Aether::Engine::MakeScope<SandboxLayer>());
        }

        void OnUpdate(float DeltaTime) override { }

    private:
        std::unique_ptr<Log::Sink> m_LogSink;
        float m_LightRotation = 0.0f;
    };

    namespace Engine {
        Application* CreateApplication(int argc, char** argv) {
            return new SandboxApp(argc, argv);
        }
    }
}
