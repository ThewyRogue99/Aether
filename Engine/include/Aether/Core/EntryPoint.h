//
// Created by ThewyRogue99 on 9.01.2026.
//

#pragma once

#include <Aether/Core/Application.h>

namespace Aether::Engine {
    extern Application* CreateApplication(int argc, char** argv);
}

int main(int argc, char** argv) {
    const auto App = Aether::Engine::CreateApplication(argc, argv);

    App->Run();

    delete App;

    return 0;
}
