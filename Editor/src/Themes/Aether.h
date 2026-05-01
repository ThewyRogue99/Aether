//
// Created by ThewyRogue99 on 15.04.2026.
//
// Aether Dark — the engine's signature dark theme.
// Cool dark grays with a soft blue accent.
//

#pragma once

#include <imgui.h>

namespace Aether::Themes {
    inline void ApplyAetherDark() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* c = style.Colors;

        // ── Base palette ─────────────────────────────────────────────
        const ImVec4 accent      = ImVec4(0.40f, 0.55f, 0.95f, 1.00f);
        const ImVec4 accentHover = ImVec4(0.50f, 0.65f, 1.00f, 1.00f);

        const ImVec4 bgDeep      = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        const ImVec4 bg          = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
        const ImVec4 bgRaised    = ImVec4(0.13f, 0.13f, 0.16f, 1.00f);
        const ImVec4 bgHover     = ImVec4(0.18f, 0.20f, 0.26f, 1.00f);
        const ImVec4 bgActive    = ImVec4(0.22f, 0.26f, 0.34f, 1.00f);

        const ImVec4 border      = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
        const ImVec4 separator   = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);

        const ImVec4 textPrimary = ImVec4(0.92f, 0.92f, 0.94f, 1.00f);
        const ImVec4 textMuted   = ImVec4(0.55f, 0.55f, 0.60f, 1.00f);

        // Text
        c[ImGuiCol_Text]                  = textPrimary;
        c[ImGuiCol_TextDisabled]          = textMuted;
        c[ImGuiCol_TextSelectedBg]        = ImVec4(accent.x, accent.y, accent.z, 0.35f);

        // Backgrounds
        c[ImGuiCol_WindowBg]              = bg;
        c[ImGuiCol_ChildBg]               = bg;
        c[ImGuiCol_PopupBg]               = bgRaised;
        c[ImGuiCol_Border]                = border;
        c[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);

        // Frames
        c[ImGuiCol_FrameBg]               = bgRaised;
        c[ImGuiCol_FrameBgHovered]        = bgHover;
        c[ImGuiCol_FrameBgActive]         = bgActive;

        // Title bars
        c[ImGuiCol_TitleBg]               = bgDeep;
        c[ImGuiCol_TitleBgActive]         = bg;
        c[ImGuiCol_TitleBgCollapsed]      = ImVec4(bgDeep.x, bgDeep.y, bgDeep.z, 0.75f);

        // Menu bar
        c[ImGuiCol_MenuBarBg]             = bg;

        // Scrollbar
        c[ImGuiCol_ScrollbarBg]           = bgDeep;
        c[ImGuiCol_ScrollbarGrab]         = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);
        c[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.32f, 0.32f, 0.36f, 1.00f);
        c[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.40f, 0.40f, 0.44f, 1.00f);

        // Interactive accents
        c[ImGuiCol_CheckMark]             = accent;
        c[ImGuiCol_SliderGrab]            = accent;
        c[ImGuiCol_SliderGrabActive]      = accentHover;

        c[ImGuiCol_Button]                = bgRaised;
        c[ImGuiCol_ButtonHovered]         = bgHover;
        c[ImGuiCol_ButtonActive]          = bgActive;

        c[ImGuiCol_Header]                = bgRaised;
        c[ImGuiCol_HeaderHovered]         = bgHover;
        c[ImGuiCol_HeaderActive]          = bgActive;

        // Separators
        c[ImGuiCol_Separator]             = separator;
        c[ImGuiCol_SeparatorHovered]      = ImVec4(accent.x, accent.y, accent.z, 0.78f);
        c[ImGuiCol_SeparatorActive]       = accent;

        // Resize grips
        c[ImGuiCol_ResizeGrip]            = ImVec4(0.20f, 0.20f, 0.24f, 0.50f);
        c[ImGuiCol_ResizeGripHovered]     = ImVec4(accent.x, accent.y, accent.z, 0.60f);
        c[ImGuiCol_ResizeGripActive]      = accent;

        // Tabs
        c[ImGuiCol_Tab]                   = bgDeep;
        c[ImGuiCol_TabHovered]            = bgHover;
        c[ImGuiCol_TabActive]             = bgActive;
        c[ImGuiCol_TabUnfocused]          = bgDeep;
        c[ImGuiCol_TabUnfocusedActive]    = bg;

        // Docking
        c[ImGuiCol_DockingPreview]        = ImVec4(accent.x, accent.y, accent.z, 0.70f);
        c[ImGuiCol_DockingEmptyBg]        = bgDeep;

        // Drag & drop
        c[ImGuiCol_DragDropTarget]        = ImVec4(accent.x, accent.y, accent.z, 0.90f);

        // Navigation / focus
        c[ImGuiCol_NavHighlight]          = accent;
        c[ImGuiCol_NavWindowingHighlight] = ImVec4(1, 1, 1, 0.70f);
        c[ImGuiCol_NavWindowingDimBg]     = ImVec4(0, 0, 0, 0.50f);
        c[ImGuiCol_ModalWindowDimBg]      = ImVec4(0, 0, 0, 0.60f);

        // Plot
        c[ImGuiCol_PlotLines]             = textMuted;
        c[ImGuiCol_PlotLinesHovered]      = accent;
        c[ImGuiCol_PlotHistogram]         = accent;
        c[ImGuiCol_PlotHistogramHovered]  = accentHover;

        // Spacing
        style.WindowPadding     = ImVec2(8.f, 8.f);
        style.FramePadding      = ImVec2(8.f, 4.f);
        style.CellPadding       = ImVec2(6.f, 4.f);
        style.ItemSpacing       = ImVec2(8.f, 4.f);
        style.ItemInnerSpacing  = ImVec2(4.f, 4.f);
        style.IndentSpacing     = 20.f;
        style.ScrollbarSize     = 12.f;
        style.GrabMinSize       = 10.f;

        // Rounding
        style.WindowRounding    = 4.f;
        style.ChildRounding     = 4.f;
        style.FrameRounding     = 3.f;
        style.PopupRounding     = 4.f;
        style.ScrollbarRounding = 3.f;
        style.GrabRounding      = 3.f;
        style.TabRounding       = 3.f;

        // Borders
        style.WindowBorderSize  = 1.f;
        style.ChildBorderSize   = 1.f;
        style.PopupBorderSize   = 1.f;
        style.FrameBorderSize   = 0.f;
        style.TabBorderSize     = 0.f;
    }
}
