//
// Created by ThewyRogue99 on 28.04.2026.
//
// Midnight Forge — modern dark/light theme with blue accents.
// Inspired by Unreal Engine 5 and modern AAA tooling.
//

#pragma once

#include <imgui.h>

namespace Aether::Themes {
    inline void ApplyMidnightForgeDark() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* c = style.Colors;

        // Base palette
        const ImVec4 bgDarkest    = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
        const ImVec4 bgDark       = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
        const ImVec4 bgMedium     = ImVec4(0.15f, 0.15f, 0.18f, 1.00f);
        const ImVec4 bgLight      = ImVec4(0.20f, 0.20f, 0.24f, 1.00f);
        const ImVec4 bgLighter    = ImVec4(0.26f, 0.26f, 0.30f, 1.00f);
        const ImVec4 borderCol    = ImVec4(0.28f, 0.28f, 0.32f, 0.60f);
        const ImVec4 textPrimary  = ImVec4(0.92f, 0.92f, 0.94f, 1.00f);
        const ImVec4 textDisabled = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);
        const ImVec4 accent       = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        const ImVec4 accentActive = ImVec4(0.18f, 0.49f, 0.88f, 1.00f);
        const ImVec4 success      = ImVec4(0.40f, 0.80f, 0.40f, 1.00f);
        const ImVec4 warning      = ImVec4(0.95f, 0.65f, 0.20f, 1.00f);

        c[ImGuiCol_Text]                  = textPrimary;
        c[ImGuiCol_TextDisabled]          = textDisabled;
        c[ImGuiCol_TextSelectedBg]        = ImVec4(accent.x, accent.y, accent.z, 0.35f);

        c[ImGuiCol_WindowBg]              = bgDark;
        c[ImGuiCol_ChildBg]               = bgDark;
        c[ImGuiCol_PopupBg]               = bgDarkest;
        c[ImGuiCol_MenuBarBg]             = bgMedium;

        c[ImGuiCol_Border]                = borderCol;
        c[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);

        c[ImGuiCol_FrameBg]               = bgMedium;
        c[ImGuiCol_FrameBgHovered]        = bgLight;
        c[ImGuiCol_FrameBgActive]         = bgLighter;

        c[ImGuiCol_TitleBg]               = bgDarkest;
        c[ImGuiCol_TitleBgActive]         = bgMedium;
        c[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.08f, 0.08f, 0.10f, 0.75f);

        c[ImGuiCol_ScrollbarBg]           = bgDarkest;
        c[ImGuiCol_ScrollbarGrab]         = bgLight;
        c[ImGuiCol_ScrollbarGrabHovered]  = bgLighter;
        c[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.35f, 0.35f, 0.40f, 1.00f);

        c[ImGuiCol_CheckMark]             = accent;
        c[ImGuiCol_SliderGrab]            = accent;
        c[ImGuiCol_SliderGrabActive]      = accentActive;

        c[ImGuiCol_Button]                = bgLight;
        c[ImGuiCol_ButtonHovered]         = accent;
        c[ImGuiCol_ButtonActive]          = accentActive;

        c[ImGuiCol_Header]                = ImVec4(accent.x, accent.y, accent.z, 0.40f);
        c[ImGuiCol_HeaderHovered]         = ImVec4(accent.x, accent.y, accent.z, 0.65f);
        c[ImGuiCol_HeaderActive]          = accent;

        c[ImGuiCol_Separator]             = borderCol;
        c[ImGuiCol_SeparatorHovered]      = ImVec4(accent.x, accent.y, accent.z, 0.78f);
        c[ImGuiCol_SeparatorActive]       = accent;

        c[ImGuiCol_ResizeGrip]            = ImVec4(accent.x, accent.y, accent.z, 0.20f);
        c[ImGuiCol_ResizeGripHovered]     = ImVec4(accent.x, accent.y, accent.z, 0.67f);
        c[ImGuiCol_ResizeGripActive]      = accent;

        c[ImGuiCol_Tab]                   = bgMedium;
        c[ImGuiCol_TabHovered]            = ImVec4(accent.x, accent.y, accent.z, 0.80f);
        c[ImGuiCol_TabActive]             = bgLight;
        c[ImGuiCol_TabUnfocused]          = bgDark;
        c[ImGuiCol_TabUnfocusedActive]    = bgMedium;

        c[ImGuiCol_DockingPreview]        = ImVec4(accent.x, accent.y, accent.z, 0.70f);
        c[ImGuiCol_DockingEmptyBg]        = bgDarkest;

        c[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        c[ImGuiCol_PlotLinesHovered]      = warning;
        c[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        c[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);

        c[ImGuiCol_TableHeaderBg]         = bgMedium;
        c[ImGuiCol_TableBorderStrong]     = borderCol;
        c[ImGuiCol_TableBorderLight]      = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        c[ImGuiCol_TableRowBg]            = ImVec4(0, 0, 0, 0);
        c[ImGuiCol_TableRowBgAlt]         = ImVec4(1, 1, 1, 0.04f);

        c[ImGuiCol_DragDropTarget]        = ImVec4(success.x, success.y, success.z, 0.90f);

        c[ImGuiCol_NavHighlight]          = accent;
        c[ImGuiCol_NavWindowingHighlight] = ImVec4(1, 1, 1, 0.70f);
        c[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        c[ImGuiCol_ModalWindowDimBg]      = ImVec4(0, 0, 0, 0.50f);

        style.WindowPadding     = ImVec2(10, 10);
        style.FramePadding      = ImVec2(8, 4);
        style.CellPadding       = ImVec2(6, 4);
        style.ItemSpacing       = ImVec2(8, 6);
        style.ItemInnerSpacing  = ImVec2(6, 4);
        style.IndentSpacing     = 22;
        style.ScrollbarSize     = 14;
        style.GrabMinSize       = 12;

        style.WindowBorderSize  = 1.f;
        style.ChildBorderSize   = 1.f;
        style.PopupBorderSize   = 1.f;
        style.FrameBorderSize   = 0.f;
        style.TabBorderSize     = 0.f;

        style.WindowRounding    = 6.f;
        style.ChildRounding     = 4.f;
        style.FrameRounding     = 4.f;
        style.PopupRounding     = 4.f;
        style.ScrollbarRounding = 9.f;
        style.GrabRounding      = 3.f;
        style.TabRounding       = 4.f;

        style.WindowTitleAlign  = ImVec2(0.f, 0.5f);
        style.ButtonTextAlign   = ImVec2(0.5f, 0.5f);

        style.AntiAliasedLines  = true;
        style.AntiAliasedFill   = true;
    }

    inline void ApplyMidnightForgeLight() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* c = style.Colors;

        // Base palette
        const ImVec4 bgLightest   = ImVec4(0.98f, 0.98f, 0.99f, 1.00f);
        const ImVec4 bgLight      = ImVec4(0.94f, 0.94f, 0.96f, 1.00f);
        const ImVec4 bgMedium     = ImVec4(0.88f, 0.88f, 0.91f, 1.00f);
        const ImVec4 bgDark       = ImVec4(0.80f, 0.80f, 0.84f, 1.00f);
        const ImVec4 bgDarker     = ImVec4(0.72f, 0.72f, 0.76f, 1.00f);
        const ImVec4 borderCol    = ImVec4(0.70f, 0.70f, 0.74f, 0.60f);
        const ImVec4 textPrimary  = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
        const ImVec4 textDisabled = ImVec4(0.55f, 0.55f, 0.58f, 1.00f);
        const ImVec4 accent       = ImVec4(0.15f, 0.45f, 0.85f, 1.00f);
        const ImVec4 accentActive = ImVec4(0.10f, 0.38f, 0.78f, 1.00f);
        const ImVec4 success      = ImVec4(0.20f, 0.65f, 0.30f, 1.00f);
        const ImVec4 warning      = ImVec4(0.85f, 0.55f, 0.10f, 1.00f);

        c[ImGuiCol_Text]                  = textPrimary;
        c[ImGuiCol_TextDisabled]          = textDisabled;
        c[ImGuiCol_TextSelectedBg]        = ImVec4(accent.x, accent.y, accent.z, 0.30f);

        c[ImGuiCol_WindowBg]              = bgLight;
        c[ImGuiCol_ChildBg]               = bgLightest;
        c[ImGuiCol_PopupBg]               = bgLightest;
        c[ImGuiCol_MenuBarBg]             = bgMedium;

        c[ImGuiCol_Border]                = borderCol;
        c[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);

        c[ImGuiCol_FrameBg]               = bgLightest;
        c[ImGuiCol_FrameBgHovered]        = ImVec4(0.92f, 0.94f, 0.98f, 1.00f);
        c[ImGuiCol_FrameBgActive]         = ImVec4(0.86f, 0.90f, 0.96f, 1.00f);

        c[ImGuiCol_TitleBg]               = bgMedium;
        c[ImGuiCol_TitleBgActive]         = bgDark;
        c[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.88f, 0.88f, 0.91f, 0.75f);

        c[ImGuiCol_ScrollbarBg]           = bgMedium;
        c[ImGuiCol_ScrollbarGrab]         = bgDark;
        c[ImGuiCol_ScrollbarGrabHovered]  = bgDarker;
        c[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.60f, 0.60f, 0.64f, 1.00f);

        c[ImGuiCol_CheckMark]             = accent;
        c[ImGuiCol_SliderGrab]            = accent;
        c[ImGuiCol_SliderGrabActive]      = accentActive;

        c[ImGuiCol_Button]                = bgMedium;
        c[ImGuiCol_ButtonHovered]         = accent;
        c[ImGuiCol_ButtonActive]          = accentActive;

        c[ImGuiCol_Header]                = ImVec4(accent.x, accent.y, accent.z, 0.30f);
        c[ImGuiCol_HeaderHovered]         = ImVec4(accent.x, accent.y, accent.z, 0.55f);
        c[ImGuiCol_HeaderActive]          = accent;

        c[ImGuiCol_Separator]             = borderCol;
        c[ImGuiCol_SeparatorHovered]      = ImVec4(accent.x, accent.y, accent.z, 0.78f);
        c[ImGuiCol_SeparatorActive]       = accent;

        c[ImGuiCol_ResizeGrip]            = ImVec4(accent.x, accent.y, accent.z, 0.20f);
        c[ImGuiCol_ResizeGripHovered]     = ImVec4(accent.x, accent.y, accent.z, 0.67f);
        c[ImGuiCol_ResizeGripActive]      = accent;

        c[ImGuiCol_Tab]                   = bgMedium;
        c[ImGuiCol_TabHovered]            = ImVec4(accent.x, accent.y, accent.z, 0.80f);
        c[ImGuiCol_TabActive]             = bgLightest;
        c[ImGuiCol_TabUnfocused]          = bgMedium;
        c[ImGuiCol_TabUnfocusedActive]    = bgLight;

        c[ImGuiCol_DockingPreview]        = ImVec4(accent.x, accent.y, accent.z, 0.70f);
        c[ImGuiCol_DockingEmptyBg]        = bgMedium;

        c[ImGuiCol_PlotLines]             = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        c[ImGuiCol_PlotLinesHovered]      = warning;
        c[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        c[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);

        c[ImGuiCol_TableHeaderBg]         = bgMedium;
        c[ImGuiCol_TableBorderStrong]     = borderCol;
        c[ImGuiCol_TableBorderLight]      = ImVec4(0.78f, 0.78f, 0.82f, 1.00f);
        c[ImGuiCol_TableRowBg]            = ImVec4(0, 0, 0, 0);
        c[ImGuiCol_TableRowBgAlt]         = ImVec4(0, 0, 0, 0.04f);

        c[ImGuiCol_DragDropTarget]        = ImVec4(success.x, success.y, success.z, 0.90f);

        c[ImGuiCol_NavHighlight]          = accent;
        c[ImGuiCol_NavWindowingHighlight] = ImVec4(0.20f, 0.20f, 0.20f, 0.70f);
        c[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
        c[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

        style.WindowPadding     = ImVec2(10, 10);
        style.FramePadding      = ImVec2(8, 4);
        style.CellPadding       = ImVec2(6, 4);
        style.ItemSpacing       = ImVec2(8, 6);
        style.ItemInnerSpacing  = ImVec2(6, 4);
        style.IndentSpacing     = 22;
        style.ScrollbarSize     = 14;
        style.GrabMinSize       = 12;

        style.WindowBorderSize  = 1.f;
        style.ChildBorderSize   = 1.f;
        style.PopupBorderSize   = 1.f;
        style.FrameBorderSize   = 0.f;
        style.TabBorderSize     = 0.f;

        style.WindowRounding    = 6.f;
        style.ChildRounding     = 4.f;
        style.FrameRounding     = 4.f;
        style.PopupRounding     = 4.f;
        style.ScrollbarRounding = 9.f;
        style.GrabRounding      = 3.f;
        style.TabRounding       = 4.f;

        style.WindowTitleAlign  = ImVec2(0.f, 0.5f);
        style.ButtonTextAlign   = ImVec2(0.5f, 0.5f);

        style.AntiAliasedLines  = true;
        style.AntiAliasedFill   = true;
    }
}
