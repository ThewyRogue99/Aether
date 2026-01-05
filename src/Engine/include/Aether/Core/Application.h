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

namespace Aether::Core {
    struct AETHER_API CommandLineArgs {
        int Count = 0;
        char** Values = nullptr;

        const char* operator[](int index) const
        {
            return Values[index];
        }
    };

    class AETHER_API Application
    {
    public:
        explicit Application(const String& name, int argc, char** argv);
        virtual ~Application();

        void Run();
        void Close();

        [[nodiscard]] Platform::Window& GetWindow() const;

    protected:
        virtual void OnInit();
        virtual void OnShutdown();
        virtual void OnUpdate(float DeltaTime);

    private:
        void Shutdown();

    private:
        String m_Name;
        CommandLineArgs m_Args;

        bool m_Running = true;
        double m_LastFrameTime = 0.0;

        Scope<Platform::Window> m_Window;
    };
}
