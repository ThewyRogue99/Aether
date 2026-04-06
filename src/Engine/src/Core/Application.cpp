//
// Created by ThewyRogue99 on 26.12.2025.
//

#include <Aether/Core/Application.h>

#include <Aether/Platform/Window.h>
#include <Aether/Platform/Platform.h>

#include <Aether/Events/Event.h>
#include <Aether/Events/EventQueue.h>
#include <Aether/Events/WindowEvent.h>
#include <Aether/Events/EventDispatcher.h>

#include <Aether/Core/Layer.h>
#include <Aether/Core/Assert.h>
#include <Aether/Core/LayerStack.h>

#include <Aether/Input/Input.h>

#include <Aether/Renderer/Renderer.h>

#include "Aether/Scene/Scene.h"

namespace {
    Aether::Engine::Application* s_Instance = nullptr;
}

namespace Aether::Engine {
    class Application::Impl {
    public:
        Impl(const String& name, int argc, char** argv) : m_Name(name), m_Args({ argc, argv }) { }

        void Init() {
            Platform::Platform::Init();

            m_Window = Platform::Window::Create(Platform::WindowProps(m_Name.c_str()));
            m_Window->SetEventQueue(&m_EventQueue);
        }

        void Run() {
            while (m_Running && !m_Window->ShouldClose()) {
                const auto currentTime = Platform::Platform::GetTime();
                const auto deltaTime = static_cast<float>(currentTime - m_LastFrameTime);

                m_LastFrameTime = currentTime;

                m_Window->OnUpdate();

                while (!m_EventQueue.Empty()) {
                    auto event = m_EventQueue.Pop();
                    s_Instance->OnEvent(*event);
                }

                for (const auto& layer : m_LayerStack)
                    layer->OnUpdate(deltaTime);

                s_Instance->OnUpdate(deltaTime);
            }
        }

        String m_Name;
        CommandLineArgs m_Args;

        bool m_Running = true;
        double m_LastFrameTime = 0.0;

        EventQueue m_EventQueue;
        LayerStack m_LayerStack;

        Scope<Scene::Scene> m_ActiveScene;

        Scope<Platform::Window> m_Window;
    };

    Application::Application(const String& name, int argc, char** argv)
        : m_Impl(MakeScope<Impl>(name, argc, argv)) {
        AETHER_ASSERT_MSG(!s_Instance, "An application already exists");
        s_Instance = this;
    }

    Application::~Application() { Shutdown(); }

    void Application::Run() {
        Init();

        m_Impl->Run();

        Shutdown();
    }

    void Application::Close() {
        m_Impl->m_Running = false;
    }

    void Application::PushLayer(Scope<Layer> layer) {
        m_Impl->m_LayerStack.PushLayer(std::move(layer));
    }

    void Application::PushOverlay(Scope<Layer> overlay) {
        m_Impl->m_LayerStack.PushOverlay(std::move(overlay));
    }

    Platform::Window& Application::GetWindow() const {
        return *m_Impl->m_Window;
    }

    Application& Application::Get() {
        AETHER_ASSERT_MSG(s_Instance, "No application initialized");
        return *s_Instance;
    }

    void Application::OnInit() { }

    void Application::OnShutdown() { }

    void Application::OnUpdate(float DeltaTime) { }

    void Application::OnEvent(Event& e) {
        Input::OnEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent&) {
            m_Impl->m_Running = false;
            return true;
        });

        dispatcher.Dispatch<FramebufferResizeEvent>([this](FramebufferResizeEvent& event) {
            Renderer::Renderer::SetViewport(event.GetWidth(), event.GetHeight());
            return false;
        });

        for (auto it = m_Impl->m_LayerStack.rbegin(); it != m_Impl->m_LayerStack.rend(); ++it) {
            (*it)->OnEvent(e);

            if (e.Handled) break;
        }
    }

    void Application::Init() {
        m_Impl->Init();

        Renderer::Renderer::Init({ m_Impl->m_Window.get() });

        OnInit();
    }

    void Application::Shutdown() {
        Renderer::Renderer::Shutdown();
        Platform::Platform::Shutdown();

        OnShutdown();
        // Add additional cleanup if necessary
    }
}
