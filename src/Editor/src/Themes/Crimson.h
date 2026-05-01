//
// Created by ThewyRogue99 on 28.04.2026.
//
// Crimson Vesuvius — deep charcoal with sharp red highlights.
//

#pragma once

#include <imgui.h>

namespace Aether::Themes {
    inline void ApplyCrimsonVesuvius() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* c = style.Colors;

        // Sizing
        style.WindowPadding     = ImVec2(10.f, 10.f);
        style.FramePadding      = ImVec2(5.f, 3.f);
        style.ItemSpacing       = ImVec2(8.f, 4.f);
        style.ScrollbarSize     = 13.f;
        style.GrabMinSize       = 10.f;

        // Borders & rounding
        style.WindowRounding    = 3.f;
        style.FrameRounding     = 2.f;
        style.PopupRounding     = 2.f;
        style.ScrollbarRounding = 12.f;
        style.GrabRounding      = 2.f;
        style.TabRounding       = 3.f;
        style.WindowBorderSize  = 1.f;
        style.FrameBorderSize   = 1.f;

        // Text
        c[ImGuiCol_Text]                  = ImVec4(1.00f, 0.90f, 0.90f, 1.00f);
        c[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.40f, 0.40f, 1.00f);

        // Backgrounds
        c[ImGuiCol_WindowBg]              = ImVec4(0.08f, 0.07f, 0.07f, 1.00f);
        c[ImGuiCol_ChildBg]               = ImVec4(0.10f, 0.09f, 0.09f, 1.00f);
        c[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.07f, 0.07f, 0.96f);

        // Borders
        c[ImGuiCol_Border]                = ImVec4(0.25f, 0.15f, 0.15f, 0.80f);
        c[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // Frames
        c[ImGuiCol_FrameBg]               = ImVec4(0.15f, 0.10f, 0.10f, 1.00f);
        c[ImGuiCol_FrameBgHovered]        = ImVec4(0.25f, 0.15f, 0.15f, 1.00f);
        c[ImGuiCol_FrameBgActive]         = ImVec4(0.35f, 0.20f, 0.20f, 1.00f);

        // Title bars
        c[ImGuiCol_TitleBg]               = ImVec4(0.12f, 0.08f, 0.08f, 1.00f);
        c[ImGuiCol_TitleBgActive]         = ImVec4(0.25f, 0.10f, 0.10f, 1.00f);
        c[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);

        c[ImGuiCol_MenuBarBg]             = ImVec4(0.12f, 0.08f, 0.08f, 1.00f);

        // Scrollbars
        c[ImGuiCol_ScrollbarBg]           = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
        c[ImGuiCol_ScrollbarGrab]         = ImVec4(0.25f, 0.12f, 0.12f, 1.00f);
        c[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.35f, 0.15f, 0.15f, 1.00f);
        c[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.45f, 0.20f, 0.20f, 1.00f);

        // Interactive
        c[ImGuiCol_CheckMark]             = ImVec4(0.85f, 0.15f, 0.15f, 1.00f);
        c[ImGuiCol_SliderGrab]            = ImVec4(0.60f, 0.12f, 0.12f, 1.00f);
        c[ImGuiCol_SliderGrabActive]      = ImVec4(0.85f, 0.15f, 0.15f, 1.00f);
        c[ImGuiCol_Button]                = ImVec4(0.30f, 0.12f, 0.12f, 1.00f);
        c[ImGuiCol_ButtonHovered]         = ImVec4(0.50f, 0.18f, 0.18f, 1.00f);
        c[ImGuiCol_ButtonActive]          = ImVec4(0.70f, 0.25f, 0.25f, 1.00f);
        c[ImGuiCol_Header]                = ImVec4(0.30f, 0.12f, 0.12f, 1.00f);
        c[ImGuiCol_HeaderHovered]         = ImVec4(0.50f, 0.18f, 0.18f, 1.00f);
        c[ImGuiCol_HeaderActive]          = ImVec4(0.70f, 0.25f, 0.25f, 1.00f);

        // Tabs
        c[ImGuiCol_Tab]                   = ImVec4(0.15f, 0.10f, 0.10f, 1.00f);
        c[ImGuiCol_TabHovered]            = ImVec4(0.50f, 0.18f, 0.18f, 1.00f);
        c[ImGuiCol_TabActive]             = ImVec4(0.35f, 0.12f, 0.12f, 1.00f);

        // Misc
        c[ImGuiCol_PlotLines]             = ImVec4(0.85f, 0.20f, 0.20f, 1.00f);
        c[ImGuiCol_TextSelectedBg]        = ImVec4(0.85f, 0.15f, 0.15f, 0.35f);
        c[ImGuiCol_NavHighlight]          = ImVec4(0.85f, 0.15f, 0.15f, 1.00f);

        c[ImGuiCol_DockingPreview]        = ImVec4(0.85f, 0.15f, 0.15f, 0.40f);
        c[ImGuiCol_DockingEmptyBg]        = ImVec4(0.08f, 0.07f, 0.07f, 1.00f);
    }
}
