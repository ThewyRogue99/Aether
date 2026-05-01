//
// Created by ThewyRogue99 on 15.04.2026.
//

#include "EditorFonts.h"

#include <imgui.h>

#include <Aether/Log/Log.h>

namespace Aether::EditorFonts {
    namespace {
        constexpr const char* kRegularFontPath = "assets/fonts/Inter-Regular.ttf";
        constexpr float kFontSize = 16.f;
    }

    void Load() {
        ImGuiIO& io = ImGui::GetIO();

        ImFontConfig config;
        config.OversampleH = 3;
        config.OversampleV = 1;
        config.PixelSnapH  = false;

        ImFont* font = io.Fonts->AddFontFromFileTTF(kRegularFontPath, kFontSize, &config);

        if (!font) {
            AETHER_WARN("Failed to load editor font '%s', falling back to ImGui default", kRegularFontPath);
            io.Fonts->AddFontDefault();
            return;
        }

        io.FontDefault = font;
    }
}
