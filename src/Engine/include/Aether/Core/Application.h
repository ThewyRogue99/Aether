//
// Created by ThewyRogue99 on 26.12.2025.
//

#pragma once

#include <Aether/Core/Core.h>
#include <Aether/Core/String.h>
#include <Aether/Core/Memory/Pointer.h>

namespace Aether::Platform {
    class Window;
}

namespace Aether::Engine {
    class Layer;
    class Event;

    struct AETHER_API CommandLineArgs {
        int Count = 0;
        char** Values = nullptr;

        const char* operator[](int index) const {
            return Values[index];
        }
    };

    class AETHER_API Application {
    public:
        explicit Application(const String& name, int argc, char** argv);
        virtual ~Application();

        void Run();
        void Close();

        void PushLayer(Scope<Layer> layer);
        void PushOverlay(Scope<Layer> overlay);

        [[nodiscard]] Platform::Window& GetWindow() const;

        static Application& Get();

    protected:
        virtual void OnInit();
        virtual void OnShutdown();
        virtual void OnUpdate(float DeltaTime);
        virtual void OnEvent(Event& e);

    private:
        void Shutdown();

    private:
        class Impl;
        Scope<Impl> m_Impl;
    };

    Application* CreateApplication(int argc, char** argv);
}
