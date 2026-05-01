//
// Gruvbox Hard — earthy, retro, terminal vibe.
//

#pragma once

#include <imgui.h>

namespace Aether::Themes {
    inline void ApplyGruvboxHard() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* c = style.Colors;

        // Sizing
        style.WindowPadding     = ImVec2(10.f, 10.f);
        style.FramePadding      = ImVec2(6.f, 4.f);
        style.ItemSpacing       = ImVec2(8.f, 4.f);
        style.ScrollbarSize     = 14.f;
        style.GrabMinSize       = 12.f;

        // Rounding (Gruvbox prefers sharp/low rounding)
        style.WindowRounding    = 2.f;
        style.FrameRounding     = 2.f;
        style.PopupRounding     = 2.f;
        style.ScrollbarRounding = 2.f;
        style.GrabRounding      = 2.f;
        style.TabRounding       = 2.f;
        style.WindowBorderSize  = 1.f;
        style.FrameBorderSize   = 1.f;
        style.PopupBorderSize   = 1.f;

        // Text
        c[ImGuiCol_Text]                  = ImVec4(0.92f, 0.86f, 0.70f, 1.00f); // #ebdbb2
        c[ImGuiCol_TextDisabled]          = ImVec4(0.57f, 0.51f, 0.45f, 1.00f); // #928374

        // Backgrounds
        c[ImGuiCol_WindowBg]              = ImVec4(0.11f, 0.13f, 0.13f, 1.00f); // #1d2021
        c[ImGuiCol_ChildBg]               = ImVec4(0.11f, 0.13f, 0.13f, 0.00f);
        c[ImGuiCol_PopupBg]               = ImVec4(0.11f, 0.13f, 0.13f, 0.95f);

        // Borders
        c[ImGuiCol_Border]                = ImVec4(0.31f, 0.29f, 0.27f, 1.00f); // #504945
        c[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // Frames
        c[ImGuiCol_FrameBg]               = ImVec4(0.24f, 0.22f, 0.21f, 1.00f); // #3c3836
        c[ImGuiCol_FrameBgHovered]        = ImVec4(0.31f, 0.29f, 0.27f, 1.00f); // #504945
        c[ImGuiCol_FrameBgActive]         = ImVec4(0.40f, 0.36f, 0.33f, 1.00f); // #665c54

        // Title bars
        c[ImGuiCol_TitleBg]               = ImVec4(0.11f, 0.13f, 0.13f, 1.00f);
        c[ImGuiCol_TitleBgActive]         = ImVec4(0.11f, 0.13f, 0.13f, 1.00f);
        c[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.11f, 0.13f, 0.13f, 1.00f);

        c[ImGuiCol_MenuBarBg]             = ImVec4(0.15f, 0.14f, 0.13f, 1.00f); // #282828

        // Scrollbars
        c[ImGuiCol_ScrollbarBg]           = ImVec4(0.11f, 0.13f, 0.13f, 1.00f);
        c[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.29f, 0.27f, 1.00f);
        c[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40f, 0.36f, 0.33f, 1.00f);
        c[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.57f, 0.51f, 0.45f, 1.00f);

        // Interactive
        c[ImGuiCol_CheckMark]             = ImVec4(0.72f, 0.73f, 0.15f, 1.00f); // #b8bb26 (green)
        c[ImGuiCol_SliderGrab]            = ImVec4(0.51f, 0.65f, 0.60f, 1.00f); // #83a598 (blue)
        c[ImGuiCol_SliderGrabActive]      = ImVec4(0.55f, 0.73f, 0.67f, 1.00f);
        c[ImGuiCol_Button]                = ImVec4(0.31f, 0.29f, 0.27f, 1.00f);
        c[ImGuiCol_ButtonHovered]         = ImVec4(0.98f, 0.29f, 0.20f, 1.00f); // #fb4934 (red)
        c[ImGuiCol_ButtonActive]          = ImVec4(0.80f, 0.20f, 0.15f, 1.00f);
        c[ImGuiCol_Header]                = ImVec4(0.24f, 0.22f, 0.21f, 1.00f);
        c[ImGuiCol_HeaderHovered]         = ImVec4(0.31f, 0.29f, 0.27f, 1.00f);
        c[ImGuiCol_HeaderActive]          = ImVec4(0.40f, 0.36f, 0.33f, 1.00f);

        // Tabs
        c[ImGuiCol_Tab]                   = ImVec4(0.24f, 0.22f, 0.21f, 1.00f);
        c[ImGuiCol_TabHovered]            = ImVec4(0.31f, 0.29f, 0.27f, 1.00f);
        c[ImGuiCol_TabActive]             = ImVec4(0.31f, 0.29f, 0.27f, 1.00f);

        // Misc
        c[ImGuiCol_PlotLines]             = ImVec4(0.98f, 0.74f, 0.18f, 1.00f); // #fabd2f (yellow)
        c[ImGuiCol_TextSelectedBg]        = ImVec4(0.31f, 0.29f, 0.27f, 1.00f);
        c[ImGuiCol_NavHighlight]          = ImVec4(0.98f, 0.29f, 0.20f, 1.00f);

        c[ImGuiCol_DockingPreview]        = ImVec4(0.72f, 0.73f, 0.15f, 0.50f);
        c[ImGuiCol_DockingEmptyBg]        = ImVec4(0.11f, 0.13f, 0.13f, 1.00f);
    }
}
