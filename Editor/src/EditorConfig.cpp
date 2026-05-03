//
// Created by ThewyRogue99 on 15.04.2026.
//

#include "EditorConfig.h"

#include <fstream>
#include <string_view>

#include <Aether/Log/Log.h>
#include <Aether/Core/String.h>

namespace Aether::EditorConfig {
    namespace {
        constexpr const char* kConfigPath = "editor.cfg";

        Engine::StringView Trim(Engine::StringView s) {
            while (!s.empty() && (s.front() == ' ' || s.front() == '\t')) s.remove_prefix(1);
            while (!s.empty() && (s.back()  == ' ' || s.back()  == '\t' ||
                s.back()  == '\r' || s.back()  == '\n')) s.remove_suffix(1);

            return s;
        }
    }

    Settings Load() {
        Settings s;

        std::ifstream file(kConfigPath);
        if (!file) return s;

        std::string line;
        while (std::getline(file, line)) {
            std::string_view view = line;
            view = Trim(view);
            if (view.empty() || view.front() == '#') continue;

            const auto eq = view.find('=');
            if (eq == std::string_view::npos) continue;

            const auto key = Trim(view.substr(0, eq));
            const auto val = Trim(view.substr(eq + 1));

            if (key == "theme") {
                s.Theme.assign(val);
            }
        }

        return s;
    }

    void Save(const Settings& s) {
        std::ofstream file(kConfigPath, std::ios::trunc);
        if (!file) {
            AETHER_WARN("Failed to write editor config to '%s'", kConfigPath);
            return;
        }

        file << "# Aether editor configuration. Edited by the editor on each change.\n";
        file << "theme=" << s.Theme.c_str() << "\n";
    }
}
