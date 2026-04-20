//
// Created by ThewyRogue99 on 12.04.2026.
//

#include <Aether/Core/EntryPoint.h>

#include <Aether/Log/Log.h>
#include <Aether/Log/StdColorSink.h>
#include <Aether/Core/Application.h>

#include "EditorLayer.h"

namespace Aether {
    class EditorApp : public Aether::Engine::Application {
    public:
        EditorApp(int argc, char** argv) : Application("Aether Editor", argc, argv),
            m_LogSink(std::make_unique<Aether::Log::StdColorSink>()) { }
        ~EditorApp() override = default;

        void OnInit() override {
            Aether::Log::SetLogger(std::make_unique<Aether::Log::Logger>("Editor"));

            auto& logger = Aether::Log::GetLogger();
            logger.AddSink(*m_LogSink);
            logger.SetLevel(Aether::Log::LogLevel::Trace);

            PushLayer(Aether::Engine::MakeScope<EditorLayer>());
        }

        void OnUpdate(float DeltaTime) override { }

    private:
        std::unique_ptr<Log::Sink> m_LogSink;
    };

    namespace Engine {
        Application* CreateApplication(int argc, char** argv) {
            return new EditorApp(argc, argv);
        }
    }
}
