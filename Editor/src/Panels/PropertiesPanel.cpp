//
// Created by ThewyRogue99 on 12.04.2026.
//

#include "PropertiesPanel.h"
#include "../EditorContext.h"

#include <imgui.h>

#include <Aether/Components/Core/Name.h>
#include <Aether/Components/Core/Transform.h>
#include <Aether/Components/Rendering/Camera.h>

namespace Aether {
    void PropertiesPanel::Draw(EditorContext& context) {
        ImGui::Begin("Properties", &m_Open);

        if (auto entity = context.SelectedEntity) {
            if (entity.HasComponent<Components::Name>()) {
                auto&[name] = entity.GetComponent<Components::Name>();
                ImGui::Text("Name: %s", name.c_str());
                ImGui::Separator();
            }

            if (entity.HasComponent<Components::Transform>()) {
                if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                    auto&[Position, Rotation, Scale] = entity.GetComponent<Components::Transform>();
                    ImGui::DragFloat3("Position", &Position.x, 0.1f);
                    ImGui::DragFloat3("Rotation", &Rotation.x, 0.5f);
                    ImGui::DragFloat3("Scale",    &Scale.x,    0.05f);
                }
            }

            if (entity.HasComponent<Components::Camera>()) {
                if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
                    auto&[FOV, Near, Far, Primary] = entity.GetComponent<Components::Camera>();
                    ImGui::DragFloat("FOV",  &FOV,  0.5f, 1.0f, 179.0f);
                    ImGui::DragFloat("Near", &Near, 0.01f, 0.001f, 100.0f);
                    ImGui::DragFloat("Far",  &Far,  1.0f, 1.0f, 10000.0f);
                    ImGui::Checkbox("Primary", &Primary);
                }
            }
        } else {
            ImGui::TextDisabled("No entity selected");
        }

        ImGui::End();
    }
} // Aether
