//
// Created by ThewyRogue99 on 12.04.2026.
//

#include "ViewportPanel.h"
#include "../EditorContext.h"

#include <imgui.h>

#include <Aether/Renderer/Renderer.h>

namespace Aether {
    void ViewportPanel::Draw(EditorContext& context) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport", &m_Open);

        const ImVec2 size = ImGui::GetContentRegionAvail();
        const auto& surface = context.ViewportSurface;

        if (size.x > 0 && size.y > 0 && surface) {
            const auto surfaceSize = Renderer::Renderer::GetRenderSurfaceSize(surface);

            if (surfaceSize.width != static_cast<uint32_t>(size.x) ||
                surfaceSize.height != static_cast<uint32_t>(size.y)) {
                Renderer::Renderer::ResizeRenderSurface(
                    surface,
                    static_cast<uint32_t>(size.x),
                    static_cast<uint32_t>(size.y)
                );
            }

            const auto colorAttachment = Renderer::Renderer::GetRenderSurfaceColorAttachment(surface);
            const auto textureID = Renderer::Renderer::GetNativeTextureID(colorAttachment);

            ImGui::Image(
                static_cast<ImTextureID>(static_cast<intptr_t>(textureID)),
                size,
                ImVec2(0, 1), ImVec2(1, 0) // flip Y for OpenGL
            );
        }

        m_IsHovered = ImGui::IsWindowHovered();

        ImGui::End();
        ImGui::PopStyleVar();
    }
} // Aether
