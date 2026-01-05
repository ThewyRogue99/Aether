//
// Created by ThewyRogue99 on 26.12.2025.
//

#include <Aether/Core/Application.h>

#include <Aether/Platform/Window.h>
#include <Aether/Platform/Platform.h>

namespace Aether::Core {
    Application::Application(const String& name, int argc, char** argv) {
        m_Args = { argc, argv };

        Platform::Platform::Init();

        m_Window = Platform::Window::Create(Platform::WindowProps(name.c_str()));
    }

    Application::~Application() {
        Shutdown();
    }

    void Application::Run() {
        OnInit();

        while (m_Running && !m_Window->ShouldClose()) {
            const auto currentTime = Platform::Platform::GetTime();
            const auto deltaTime = static_cast<float>(currentTime - m_LastFrameTime);

            m_LastFrameTime = currentTime;

            m_Window->OnUpdate();
            OnUpdate(deltaTime);
        }

        Shutdown();
    }

    void Application::Close() {
        m_Running = false;
    }

    Platform::Window& Application::GetWindow() const {
        return *m_Window;
    }

    void Application::OnInit() { }

    void Application::OnShutdown() { }

    void Application::OnUpdate(float DeltaTime) { }

    void Application::Shutdown() {
        OnShutdown();
        // Add additional cleanup if necessary
    }
}
