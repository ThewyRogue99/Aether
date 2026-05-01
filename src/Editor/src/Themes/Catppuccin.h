//
// Catppuccin theme for Dear ImGui — 4 official flavors.
// Palette reference: https://github.com/catppuccin/catppuccin
//

#pragma once

#include <imgui.h>

namespace Aether::Themes {
    namespace detail::catppuccin {
        struct Palette {
            ImVec4 rosewater, flamingo, pink, mauve, red, maroon, peach, yellow,
                   green, teal, sky, sapphire, blue, lavender,
                   text, subtext1, subtext0, overlay2, overlay1, overlay0,
                   surface2, surface1, surface0, base, mantle, crust;
        };

        inline ImVec4 RGBA(int r, int g, int b, float a = 1.0f) {
            return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a);
        }

        inline void Apply(const Palette& p) {
            ImGuiStyle& style = ImGui::GetStyle();
            ImVec4* c = style.Colors;

            auto with_alpha = [](ImVec4 v, float a) { v.w = a; return v; };

            c[ImGuiCol_Text]                  = p.text;
            c[ImGuiCol_TextDisabled]          = p.overlay0;
            c[ImGuiCol_WindowBg]              = p.base;
            c[ImGuiCol_ChildBg]               = p.mantle;
            c[ImGuiCol_PopupBg]               = p.mantle;
            c[ImGuiCol_Border]                = p.surface0;
            c[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);

            c[ImGuiCol_FrameBg]               = p.surface0;
            c[ImGuiCol_FrameBgHovered]        = p.surface1;
            c[ImGuiCol_FrameBgActive]         = p.surface2;

            c[ImGuiCol_TitleBg]               = p.crust;
            c[ImGuiCol_TitleBgActive]         = p.mantle;
            c[ImGuiCol_TitleBgCollapsed]      = p.crust;

            c[ImGuiCol_MenuBarBg]             = p.mantle;

            c[ImGuiCol_ScrollbarBg]           = p.mantle;
            c[ImGuiCol_ScrollbarGrab]         = p.surface1;
            c[ImGuiCol_ScrollbarGrabHovered]  = p.surface2;
            c[ImGuiCol_ScrollbarGrabActive]   = p.overlay0;

            c[ImGuiCol_CheckMark]             = p.mauve;
            c[ImGuiCol_SliderGrab]            = p.mauve;
            c[ImGuiCol_SliderGrabActive]      = p.lavender;

            c[ImGuiCol_Button]                = with_alpha(p.mauve, 0.40f);
            c[ImGuiCol_ButtonHovered]         = with_alpha(p.mauve, 0.65f);
            c[ImGuiCol_ButtonActive]          = p.mauve;

            c[ImGuiCol_Header]                = with_alpha(p.blue, 0.35f);
            c[ImGuiCol_HeaderHovered]         = with_alpha(p.blue, 0.60f);
            c[ImGuiCol_HeaderActive]          = p.blue;

            c[ImGuiCol_Separator]             = p.surface1;
            c[ImGuiCol_SeparatorHovered]      = p.overlay0;
            c[ImGuiCol_SeparatorActive]       = p.overlay1;

            c[ImGuiCol_ResizeGrip]            = with_alpha(p.mauve, 0.25f);
            c[ImGuiCol_ResizeGripHovered]     = with_alpha(p.mauve, 0.55f);
            c[ImGuiCol_ResizeGripActive]      = p.mauve;

            c[ImGuiCol_Tab]                   = p.surface0;
            c[ImGuiCol_TabHovered]            = p.surface2;
            c[ImGuiCol_TabActive]             = p.surface1;
            c[ImGuiCol_TabUnfocused]          = p.crust;
            c[ImGuiCol_TabUnfocusedActive]    = p.mantle;

            c[ImGuiCol_DockingPreview]        = with_alpha(p.mauve, 0.70f);
            c[ImGuiCol_DockingEmptyBg]        = p.crust;

            c[ImGuiCol_PlotLines]             = p.blue;
            c[ImGuiCol_PlotLinesHovered]      = p.sapphire;
            c[ImGuiCol_PlotHistogram]         = p.peach;
            c[ImGuiCol_PlotHistogramHovered]  = p.yellow;

            c[ImGuiCol_TableHeaderBg]         = p.mantle;
            c[ImGuiCol_TableBorderStrong]     = p.surface1;
            c[ImGuiCol_TableBorderLight]      = p.surface0;
            c[ImGuiCol_TableRowBg]            = ImVec4(0, 0, 0, 0);
            c[ImGuiCol_TableRowBgAlt]         = with_alpha(p.surface0, 0.30f);

            c[ImGuiCol_TextSelectedBg]        = with_alpha(p.mauve, 0.40f);
            c[ImGuiCol_DragDropTarget]        = p.yellow;
            c[ImGuiCol_NavHighlight]          = p.mauve;
            c[ImGuiCol_NavWindowingHighlight] = with_alpha(p.text, 0.70f);
            c[ImGuiCol_NavWindowingDimBg]     = with_alpha(p.crust, 0.50f);
            c[ImGuiCol_ModalWindowDimBg]      = with_alpha(p.crust, 0.60f);

            style.WindowRounding    = 6.0f;
            style.ChildRounding     = 6.0f;
            style.FrameRounding     = 4.0f;
            style.PopupRounding     = 4.0f;
            style.ScrollbarRounding = 6.0f;
            style.GrabRounding      = 4.0f;
            style.TabRounding       = 4.0f;
            style.WindowBorderSize  = 1.0f;
            style.FrameBorderSize   = 0.0f;
            style.PopupBorderSize   = 1.0f;
        }
    } // namespace detail::catppuccin

    inline void ApplyCatppuccinLatte() {
        using namespace detail::catppuccin;
        Apply(Palette {
            RGBA(220, 138, 120), RGBA(221, 120, 120), RGBA(234, 118, 203),
            RGBA(136,  57, 239), RGBA(210,  15,  57), RGBA(230,  69,  83),
            RGBA(254, 100,  11), RGBA(223, 142,  29), RGBA( 64, 160,  43),
            RGBA( 23, 146, 153), RGBA(  4, 165, 229), RGBA( 32, 159, 181),
            RGBA( 30, 102, 245), RGBA(114, 135, 253), RGBA( 76,  79, 105),
            RGBA( 92,  95, 119), RGBA(108, 111, 133), RGBA(124, 127, 147),
            RGBA(140, 143, 161), RGBA(156, 160, 176), RGBA(172, 176, 190),
            RGBA(188, 192, 204), RGBA(204, 208, 218), RGBA(239, 241, 245),
            RGBA(230, 233, 239), RGBA(220, 224, 232),
        });
    }

    inline void ApplyCatppuccinFrappe() {
        using namespace detail::catppuccin;
        Apply(Palette {
            RGBA(242, 213, 207), RGBA(238, 190, 190), RGBA(244, 184, 228),
            RGBA(202, 158, 230), RGBA(231, 130, 132), RGBA(234, 153, 156),
            RGBA(239, 159, 118), RGBA(229, 200, 144), RGBA(166, 209, 137),
            RGBA(129, 200, 190), RGBA(153, 209, 219), RGBA(133, 193, 220),
            RGBA(140, 170, 238), RGBA(186, 187, 241), RGBA(198, 208, 245),
            RGBA(181, 191, 226), RGBA(165, 173, 206), RGBA(148, 156, 187),
            RGBA(131, 139, 167), RGBA(115, 121, 148), RGBA( 98, 104, 128),
            RGBA( 81,  87, 109), RGBA( 65,  69,  89), RGBA( 48,  52,  70),
            RGBA( 41,  44,  60), RGBA( 35,  38,  52),
        });
    }

    inline void ApplyCatppuccinMacchiato() {
        using namespace detail::catppuccin;
        Apply(Palette {
            RGBA(244, 219, 214), RGBA(240, 198, 198), RGBA(245, 189, 230),
            RGBA(198, 160, 246), RGBA(237, 135, 150), RGBA(238, 153, 160),
            RGBA(245, 169, 127), RGBA(238, 212, 159), RGBA(166, 218, 149),
            RGBA(139, 213, 202), RGBA(145, 215, 227), RGBA(125, 196, 228),
            RGBA(138, 173, 244), RGBA(183, 189, 248), RGBA(202, 211, 245),
            RGBA(184, 192, 224), RGBA(165, 173, 203), RGBA(147, 154, 183),
            RGBA(128, 135, 162), RGBA(110, 115, 141), RGBA( 91,  96, 120),
            RGBA( 73,  77, 100), RGBA( 54,  58,  79), RGBA( 36,  39,  58),
            RGBA( 30,  32,  48), RGBA( 24,  25,  38),
        });
    }

    inline void ApplyCatppuccinMocha() {
        using namespace detail::catppuccin;
        Apply(Palette {
            RGBA(245, 224, 220), RGBA(242, 205, 205), RGBA(245, 194, 231),
            RGBA(203, 166, 247), RGBA(243, 139, 168), RGBA(235, 160, 172),
            RGBA(250, 179, 135), RGBA(249, 226, 175), RGBA(166, 227, 161),
            RGBA(148, 226, 213), RGBA(137, 220, 235), RGBA(116, 199, 236),
            RGBA(137, 180, 250), RGBA(180, 190, 254), RGBA(205, 214, 244),
            RGBA(186, 194, 222), RGBA(166, 173, 200), RGBA(147, 153, 178),
            RGBA(127, 132, 156), RGBA(108, 112, 134), RGBA( 88,  91, 112),
            RGBA( 69,  71,  90), RGBA( 49,  50,  68), RGBA( 30,  30,  46),
            RGBA( 24,  24,  37), RGBA( 17,  17,  27),
        });
    }
}
