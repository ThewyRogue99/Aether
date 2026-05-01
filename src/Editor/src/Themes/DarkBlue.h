//
// Dark Blue — Visual Studio-like rounded dark theme.
// Source: RedNicStone via ImThemes.
//

#pragma once

#include <imgui.h>

namespace Aether::Themes {
    inline void ApplyDarkBlue() {
        ImGuiStyle& style = ImGui::GetStyle();

        style.Alpha                   = 1.0f;
        style.DisabledAlpha           = 0.6f;
        style.WindowPadding           = ImVec2(8.f, 8.f);
        style.WindowRounding          = 4.f;
        style.WindowBorderSize        = 0.f;
        style.WindowMinSize           = ImVec2(32.f, 32.f);
        style.WindowTitleAlign        = ImVec2(0.f, 0.5f);
        style.WindowMenuButtonPosition= ImGuiDir_Left;
        style.ChildRounding           = 0.f;
        style.ChildBorderSize         = 1.f;
        style.PopupRounding           = 4.f;
        style.PopupBorderSize         = 1.f;
        style.FramePadding            = ImVec2(4.f, 3.f);
        style.FrameRounding           = 2.5f;
        style.FrameBorderSize         = 0.f;
        style.ItemSpacing             = ImVec2(8.f, 4.f);
        style.ItemInnerSpacing        = ImVec2(4.f, 4.f);
        style.CellPadding             = ImVec2(4.f, 2.f);
        style.IndentSpacing           = 21.f;
        style.ColumnsMinSpacing       = 6.f;
        style.ScrollbarSize           = 11.f;
        style.ScrollbarRounding       = 2.5f;
        style.GrabMinSize             = 10.f;
        style.GrabRounding            = 2.f;
        style.TabRounding             = 3.5f;
        style.TabBorderSize           = 0.f;
        style.ColorButtonPosition     = ImGuiDir_Right;
        style.ButtonTextAlign         = ImVec2(0.5f, 0.5f);
        style.SelectableTextAlign     = ImVec2(0.f, 0.f);

        ImVec4* c = style.Colors;
        c[ImGuiCol_Text]                  = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
        c[ImGuiCol_TextDisabled]          = ImVec4(0.592f, 0.592f, 0.592f, 1.000f);
        c[ImGuiCol_WindowBg]              = ImVec4(0.145f, 0.145f, 0.149f, 1.000f);
        c[ImGuiCol_ChildBg]               = ImVec4(0.145f, 0.145f, 0.149f, 1.000f);
        c[ImGuiCol_PopupBg]               = ImVec4(0.145f, 0.145f, 0.149f, 1.000f);
        c[ImGuiCol_Border]                = ImVec4(0.306f, 0.306f, 0.306f, 1.000f);
        c[ImGuiCol_BorderShadow]          = ImVec4(0.306f, 0.306f, 0.306f, 1.000f);
        c[ImGuiCol_FrameBg]               = ImVec4(0.200f, 0.200f, 0.216f, 1.000f);
        c[ImGuiCol_FrameBgHovered]        = ImVec4(0.114f, 0.592f, 0.925f, 1.000f);
        c[ImGuiCol_FrameBgActive]         = ImVec4(0.000f, 0.467f, 0.784f, 1.000f);
        c[ImGuiCol_TitleBg]               = ImVec4(0.145f, 0.145f, 0.149f, 1.000f);
        c[ImGuiCol_TitleBgActive]         = ImVec4(0.145f, 0.145f, 0.149f, 1.000f);
        c[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.145f, 0.145f, 0.149f, 1.000f);
        c[ImGuiCol_MenuBarBg]             = ImVec4(0.200f, 0.200f, 0.216f, 1.000f);
        c[ImGuiCol_ScrollbarBg]           = ImVec4(0.200f, 0.200f, 0.216f, 1.000f);
        c[ImGuiCol_ScrollbarGrab]         = ImVec4(0.322f, 0.322f, 0.333f, 1.000f);
        c[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.353f, 0.353f, 0.373f, 1.000f);
        c[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.353f, 0.353f, 0.373f, 1.000f);
        c[ImGuiCol_CheckMark]             = ImVec4(0.000f, 0.467f, 0.784f, 1.000f);
        c[ImGuiCol_SliderGrab]            = ImVec4(0.114f, 0.592f, 0.925f, 1.000f);
        c[ImGuiCol_SliderGrabActive]      = ImVec4(0.000f, 0.467f, 0.784f, 1.000f);
        c[ImGuiCol_Button]                = ImVec4(0.200f, 0.200f, 0.216f, 1.000f);
        c[ImGuiCol_ButtonHovered]         = ImVec4(0.114f, 0.592f, 0.925f, 1.000f);
        c[ImGuiCol_ButtonActive]          = ImVec4(0.114f, 0.592f, 0.925f, 1.000f);
        c[ImGuiCol_Header]                = ImVec4(0.200f, 0.200f, 0.216f, 1.000f);
        c[ImGuiCol_HeaderHovered]         = ImVec4(0.114f, 0.592f, 0.925f, 1.000f);
        c[ImGuiCol_HeaderActive]          = ImVec4(0.000f, 0.467f, 0.784f, 1.000f);
        c[ImGuiCol_Separator]             = ImVec4(0.306f, 0.306f, 0.306f, 1.000f);
        c[ImGuiCol_SeparatorHovered]      = ImVec4(0.306f, 0.306f, 0.306f, 1.000f);
        c[ImGuiCol_SeparatorActive]       = ImVec4(0.306f, 0.306f, 0.306f, 1.000f);
        c[ImGuiCol_ResizeGrip]            = ImVec4(0.145f, 0.145f, 0.149f, 1.000f);
        c[ImGuiCol_ResizeGripHovered]     = ImVec4(0.200f, 0.200f, 0.216f, 1.000f);
        c[ImGuiCol_ResizeGripActive]      = ImVec4(0.322f, 0.322f, 0.333f, 1.000f);
        c[ImGuiCol_Tab]                   = ImVec4(0.145f, 0.145f, 0.149f, 1.000f);
        c[ImGuiCol_TabHovered]            = ImVec4(0.114f, 0.592f, 0.925f, 1.000f);
        c[ImGuiCol_TabActive]             = ImVec4(0.000f, 0.467f, 0.784f, 1.000f);
        c[ImGuiCol_TabUnfocused]          = ImVec4(0.145f, 0.145f, 0.149f, 1.000f);
        c[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.000f, 0.467f, 0.784f, 1.000f);
        c[ImGuiCol_PlotLines]             = ImVec4(0.000f, 0.467f, 0.784f, 1.000f);
        c[ImGuiCol_PlotLinesHovered]      = ImVec4(0.114f, 0.592f, 0.925f, 1.000f);
        c[ImGuiCol_PlotHistogram]         = ImVec4(0.000f, 0.467f, 0.784f, 1.000f);
        c[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.114f, 0.592f, 0.925f, 1.000f);
        c[ImGuiCol_TableHeaderBg]         = ImVec4(0.188f, 0.188f, 0.200f, 1.000f);
        c[ImGuiCol_TableBorderStrong]     = ImVec4(0.310f, 0.310f, 0.349f, 1.000f);
        c[ImGuiCol_TableBorderLight]      = ImVec4(0.227f, 0.227f, 0.247f, 1.000f);
        c[ImGuiCol_TableRowBg]            = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
        c[ImGuiCol_TableRowBgAlt]         = ImVec4(1.000f, 1.000f, 1.000f, 0.060f);
        c[ImGuiCol_TextSelectedBg]        = ImVec4(0.000f, 0.467f, 0.784f, 1.000f);
        c[ImGuiCol_DragDropTarget]        = ImVec4(0.145f, 0.145f, 0.149f, 1.000f);
        c[ImGuiCol_NavHighlight]          = ImVec4(0.145f, 0.145f, 0.149f, 1.000f);
        c[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 1.000f, 1.000f, 0.700f);
        c[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.800f, 0.800f, 0.800f, 0.200f);
        c[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.145f, 0.145f, 0.149f, 1.000f);
    }
}
