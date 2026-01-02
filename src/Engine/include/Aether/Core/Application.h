//
// Created by ThewyRogue99 on 26.12.2025.
//

#pragma once

#include "Core.h"

namespace Aether::Core {
    class AETHER_API Application {
    public:
        virtual ~Application();

        void Run();

    protected:
        Application();

        virtual void OnInit();
        virtual void OnUpdate(float deltaTime);
        virtual void OnShutdown();
    };
}
