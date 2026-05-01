//
// Created by ThewyRogue99 on 12.04.2026.
//

#include "SceneHierarchyPanel.h"

#include <imgui.h>

#include <Aether/Scene/Scene.h>
#include <Aether/Components/Core/Name.h>
#include <Aether/Components/Core/Transform.h>
#include <Aether/ECS/Entity.h>

#include "EditorContext.h"
#include "Components/EditorCamera.h"

namespace Aether {
    SceneHierarchyPanel::SceneHierarchyPanel() = default;

    SceneHierarchyPanel::~SceneHierarchyPanel() = default;

    void SceneHierarchyPanel::OnAttach(EditorContext& context) {
        EditorPanel::OnAttach(context);
    }

    void SceneHierarchyPanel::OnDetach(EditorContext& context) {
        EditorPanel::OnDetach(context);
    }

    void SceneHierarchyPanel::Draw(EditorContext& context) {
        ImGui::Begin("Scene Hierarchy", &m_Open);

        auto* activeScene = context.ActiveScene;

        Scene::Entity entityToDelete;
        bool createEmptyRequested = false;

        if (activeScene) {
            for (auto [entity, transform] : activeScene->View<Components::Transform>().each()) {
                Scene::Entity sceneEntity(entity, activeScene);

                // Editor-only entities (e.g. the editor camera) shouldn't appear here.
                if (sceneEntity.HasComponent<Components::EditorCamera>()) continue;

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

                // Per-entity right-click menu.
                if (ImGui::BeginPopupContextItem()) {
                    context.SelectedEntity = sceneEntity; // selecting on right-click feels natural

                    if (ImGui::MenuItem("Delete Entity")) {
                        entityToDelete = sceneEntity;
                    }
                    ImGui::EndPopup();
                }
            }
        }

        // Right-click on empty space inside the panel → "create" menu.
        // NoOpenOverItems ensures this doesn't fire on top of an entity row
        // (which has its own per-item popup above).
        if (
            activeScene &&
            ImGui::BeginPopupContextWindow(
                nullptr,
                ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems
            )
        ) {
            if (ImGui::MenuItem("Create Empty Entity")) {
                createEmptyRequested = true;
            }
            ImGui::EndPopup();
        }

        // Click on empty space → deselect.
        if (
            ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
            ImGui::IsWindowHovered() &&
            !ImGui::IsAnyItemHovered()
        ) {
            context.SelectedEntity = { };
        }

        ImGui::End();

        // ── Deferred mutations ───────────────────────────────────────
        if (activeScene && createEmptyRequested) {
            auto entity = activeScene->CreateEntity("Entity");
            entity.AddComponent<Components::Transform>();
            context.SelectedEntity = entity;
        }

        if (activeScene && entityToDelete) {
            if (context.SelectedEntity == entityToDelete) {
                context.SelectedEntity = {};
            }
            activeScene->DestroyEntity(entityToDelete);
        }
    }
} // Aether
