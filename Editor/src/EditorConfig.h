//
// Created by ThewyRogue99 on 15.04.2026.
//
// Persistent editor settings stored in a small key=value config file
// next to the editor binary. Load on startup, save on change.
//

#pragma once

#include <Aether/Core/String.h>

namespace Aether::EditorConfig {
    struct Settings {
        Engine::String Theme = "Aether Dark";
    };

    // Load settings from the config file. Missing or unparseable file
    // returns defaults.
    Settings Load();

    // Persist settings to the config file. Failures are logged, not thrown.
    void Save(const Settings& settings);
}
