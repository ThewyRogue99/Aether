//
// Created by ThewyRogue99 on 12.04.2026.
//

#include "SceneHierarchyPanel.h"

#include <imgui.h>

#include <Aether/Scene/Scene.h>
#include <Aether/Components/Core/Name.h>
#include <Aether/Components/Core/Transform.h>
#include <Aether/ECS/Entity.h>
#include <Aether/Core/Application.h>
#include <Aether/Scene/SceneManager.h>

#include "EditorContext.h"

namespace Aether {
    SceneHierarchyPanel::SceneHierarchyPanel() = default;

    SceneHierarchyPanel::~SceneHierarchyPanel() = default;

    void SceneHierarchyPanel::OnAttach(EditorContext& context) {
        EditorPanel::OnAttach(context);
    }

    void SceneHierarchyPanel::OnDetach(EditorContext &context) {
        EditorPanel::OnDetach(context);
    }

    void SceneHierarchyPanel::Draw(EditorContext& context) {
        ImGui::Begin("Scene Hierarchy", &m_Open);

        if (const auto activeScene = context.ActiveScene) {
            for (auto [entity, transform] : activeScene->View<Components::Transform>().each()) {
                Scene::Entity sceneEntity(entity, activeScene);

                const char* label = "Entity";
                if (sceneEntity.HasComponent<Components::Name>()) {
                    label = sceneEntity.GetComponent<Components::Name>().Value.c_str();
                }

                char buf[64];
                snprintf(buf, sizeof(buf), "%s##%u", label, ECS::EntityIndex(entity));

                const bool isSelected = (context.SelectedEntity == sceneEntity);

                if (ImGui::Selectable(buf, isSelected)) {
                    context.SelectedEntity = sceneEntity;
                }
            }
        }

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
            context.SelectedEntity = {};
        }

        ImGui::End();
    }
} // Aether
