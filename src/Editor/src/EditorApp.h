//
// Created by ThewyRogue99 on 5.01.2026.
//

#pragma once

#include <Aether/Core/Application.h>

namespace Aether::Editor {
    class EditorApp : public Core::Application {
    public:
        EditorApp(int argc, char** argv);
        ~EditorApp() override;
    };
}
