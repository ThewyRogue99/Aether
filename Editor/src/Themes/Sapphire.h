//
// Created by ThewyRogue99 on 28.04.2026.
//
// Sapphire — deep midnight blues with bright cyan highlights.
//

#pragma once

#include <imgui.h>

namespace Aether::Themes {
    inline void ApplySapphire() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* c = style.Colors;

        // Sizing
        style.WindowPadding     = ImVec2(10.f, 10.f);
        style.FramePadding      = ImVec2(6.f, 4.f);
        style.ItemSpacing       = ImVec2(8.f, 4.f);
        style.ScrollbarSize     = 15.f;
        style.GrabMinSize       = 10.f;

        // Borders & rounding
        style.WindowRounding    = 5.f;
        style.FrameRounding     = 4.f;
        style.PopupRounding     = 4.f;
        style.ScrollbarRounding = 12.f;
        style.GrabRounding      = 3.f;
        style.TabRounding       = 4.f;
        style.WindowBorderSize  = 1.f;
        style.FrameBorderSize   = 1.f;

        // Text
        c[ImGuiCol_Text]                  = ImVec4(0.90f, 0.93f, 0.97f, 1.00f);
        c[ImGuiCol_TextDisabled]          = ImVec4(0.40f, 0.50f, 0.65f, 1.00f);

        // Backgrounds
        c[ImGuiCol_WindowBg]              = ImVec4(0.07f, 0.09f, 0.12f, 1.00f);
        c[ImGuiCol_ChildBg]               = ImVec4(0.09f, 0.12f, 0.16f, 1.00f);
        c[ImGuiCol_PopupBg]               = ImVec4(0.07f, 0.09f, 0.12f, 0.95f);

        // Borders
        c[ImGuiCol_Border]                = ImVec4(0.15f, 0.25f, 0.35f, 0.70f);
        c[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // Frames
        c[ImGuiCol_FrameBg]               = ImVec4(0.12f, 0.18f, 0.26f, 1.00f);
        c[ImGuiCol_FrameBgHovered]        = ImVec4(0.18f, 0.28f, 0.40f, 1.00f);
        c[ImGuiCol_FrameBgActive]         = ImVec4(0.25f, 0.38f, 0.55f, 1.00f);

        // Title bars
        c[ImGuiCol_TitleBg]               = ImVec4(0.09f, 0.12f, 0.18f, 1.00f);
        c[ImGuiCol_TitleBgActive]         = ImVec4(0.14f, 0.22f, 0.35f, 1.00f);
        c[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.05f, 0.08f, 0.12f, 1.00f);

        c[ImGuiCol_MenuBarBg]             = ImVec4(0.12f, 0.16f, 0.22f, 1.00f);

        // Scrollbars
        c[ImGuiCol_ScrollbarBg]           = ImVec4(0.06f, 0.08f, 0.11f, 1.00f);
        c[ImGuiCol_ScrollbarGrab]         = ImVec4(0.20f, 0.32f, 0.48f, 1.00f);
        c[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.28f, 0.42f, 0.60f, 1.00f);
        c[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.35f, 0.50f, 0.75f, 1.00f);

        // Interactive
        c[ImGuiCol_CheckMark]             = ImVec4(0.40f, 0.70f, 1.00f, 1.00f);
        c[ImGuiCol_SliderGrab]            = ImVec4(0.30f, 0.55f, 0.85f, 1.00f);
        c[ImGuiCol_SliderGrabActive]      = ImVec4(0.45f, 0.75f, 1.00f, 1.00f);
        c[ImGuiCol_Button]                = ImVec4(0.18f, 0.35f, 0.55f, 1.00f);
        c[ImGuiCol_ButtonHovered]         = ImVec4(0.25f, 0.48f, 0.75f, 1.00f);
        c[ImGuiCol_ButtonActive]          = ImVec4(0.35f, 0.60f, 0.90f, 1.00f);
        c[ImGuiCol_Header]                = ImVec4(0.18f, 0.35f, 0.55f, 1.00f);
        c[ImGuiCol_HeaderHovered]         = ImVec4(0.25f, 0.48f, 0.75f, 1.00f);
        c[ImGuiCol_HeaderActive]          = ImVec4(0.35f, 0.60f, 0.90f, 1.00f);

        // Tabs
        c[ImGuiCol_Tab]                   = ImVec4(0.12f, 0.20f, 0.32f, 1.00f);
        c[ImGuiCol_TabHovered]            = ImVec4(0.25f, 0.45f, 0.70f, 1.00f);
        c[ImGuiCol_TabActive]             = ImVec4(0.18f, 0.35f, 0.55f, 1.00f);
        c[ImGuiCol_TabUnfocused]          = ImVec4(0.08f, 0.12f, 0.18f, 1.00f);
        c[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.12f, 0.20f, 0.32f, 1.00f);

        // Tables
        c[ImGuiCol_TableHeaderBg]         = ImVec4(0.15f, 0.25f, 0.40f, 1.00f);
        c[ImGuiCol_TableBorderStrong]     = ImVec4(0.20f, 0.35f, 0.55f, 1.00f);
        c[ImGuiCol_TableBorderLight]      = ImVec4(0.15f, 0.25f, 0.40f, 1.00f);
        c[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.05f);

        // Misc
        c[ImGuiCol_TextSelectedBg]        = ImVec4(0.30f, 0.55f, 0.85f, 0.40f);
        c[ImGuiCol_DragDropTarget]        = ImVec4(0.50f, 0.80f, 1.00f, 0.90f);
        c[ImGuiCol_NavHighlight]          = ImVec4(0.40f, 0.70f, 1.00f, 1.00f);

        c[ImGuiCol_DockingPreview]        = ImVec4(0.25f, 0.50f, 0.80f, 0.50f);
        c[ImGuiCol_DockingEmptyBg]        = ImVec4(0.07f, 0.09f, 0.12f, 1.00f);
    }
}
