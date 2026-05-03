//
// Created by ThewyRogue99 on 12.04.2026.
//

#include "PropertiesPanel.h"
#include "../EditorContext.h"

#include <imgui.h>

#include <cstdio>
#include <cstring>

#include <Aether/Components/Core/Name.h>
#include <Aether/Components/Core/Tag.h>
#include <Aether/Components/Core/Transform.h>
#include <Aether/Components/Physics/Velocity.h>
#include <Aether/Components/Rendering/Camera.h>
#include <Aether/Components/Rendering/MeshRenderer.h>

namespace Aether {
    namespace {
        // ── Helpers ──────────────────────────────────────────────────

        // CollapsingHeader + right-click "Remove Component" + body callback,
        // all in one. Body runs only if the header is open. The remove call
        // happens after the body returns so the reference handed to body is
        // not in use when storage is mutated.
        template<typename T, typename DrawBody>
        void DrawComponent(Scene::Entity entity, const char* label, DrawBody body) {
            if (!entity.HasComponent<T>()) return;

            const bool open = ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen);

            bool wantRemove = false;
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Remove Component")) wantRemove = true;
                ImGui::EndPopup();
            }

            if (open) {
                body(entity.GetComponent<T>());
            }

            if (wantRemove) {
                entity.RemoveComponent<T>();
            }
        }

        template<typename T>
        void AddComponentMenuItem(Scene::Entity entity, const char* label) {
            if (entity.HasComponent<T>()) return;
            if (ImGui::MenuItem(label)) {
                entity.AddComponent<T>();
                ImGui::CloseCurrentPopup();
            }
        }

        // ── Inline Name editor (no header, lives at the top) ─────────
        void DrawNameEditor(Scene::Entity entity) {
            if (!entity.HasComponent<Components::Name>()) return;

            auto&[name] = entity.GetComponent<Components::Name>();

            char buffer[128];
            const auto& src = name;
            const auto copyLen = src.size() < sizeof(buffer) - 1
                ? src.size()
                : static_cast<uint32_t>(sizeof(buffer) - 1);
            std::memcpy(buffer, src.data(), copyLen);
            buffer[copyLen] = '\0';

            if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
                name.assign(buffer);
            }
        }

        // ── Component editors (passed as lambdas to DrawComponent) ───

        void DrawTransformBody(Components::Transform& t) {
            ImGui::DragFloat3("Position", &t.Position.x, 0.1f);
            ImGui::DragFloat3("Rotation", &t.Rotation.x, 0.5f);
            ImGui::DragFloat3("Scale", &t.Scale.x, 0.05f);
        }

        void DrawCameraBody(Components::Camera& cam) {
            ImGui::DragFloat("FOV", &cam.FOV, 0.5f, 1.0f, 179.0f);
            ImGui::DragFloat("Near", &cam.Near,0.01f, 0.001f, 100.0f);
            ImGui::DragFloat("Far", &cam.Far, 1.0f, 1.0f,10000.0f);
            ImGui::Checkbox("Primary", &cam.Primary);
        }

        void DrawVelocityBody(Components::Velocity& v) {
            ImGui::DragFloat3("Linear", &v.Value.x, 0.1f);
        }

        void DrawTagBody(Components::Tag& tag) {
            // Defer mutations — don't mutate the vector while iterating it.
            int rowToRemove = -1;
            bool addRequested = false;

            for (size_t i = 0; i < tag.Tags.size(); ++i) {
                ImGui::PushID(static_cast<int>(i));

                auto& s = tag.Tags[i];

                char buffer[128];
                const auto copyLen = s.size() < sizeof(buffer) - 1
                    ? s.size()
                    : static_cast<uint32_t>(sizeof(buffer) - 1);
                std::memcpy(buffer, s.data(), copyLen);
                buffer[copyLen] = '\0';

                // Leave room for the "X" button.
                const float removeBtnW = ImGui::GetFrameHeight();
                const float spacing = ImGui::GetStyle().ItemSpacing.x;
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - removeBtnW - spacing);

                if (ImGui::InputText("##tag", buffer, sizeof(buffer))) {
                    s.assign(buffer);
                }
                ImGui::SameLine();
                if (ImGui::Button("X", ImVec2(removeBtnW, 0))) {
                    rowToRemove = static_cast<int>(i);
                }

                ImGui::PopID();
            }

            if (ImGui::Button("+ Add Tag")) addRequested = true;

            if (rowToRemove >= 0) {
                tag.Tags.erase(tag.Tags.begin() + rowToRemove);
            }
            if (addRequested) {
                tag.Tags.emplace_back();
            }
        }

        void DrawMeshRendererBody(Components::MeshRenderer& mr) {
            ImGui::TextUnformatted("Mesh");
            ImGui::Indent();
            ImGui::Text(
                "Vertex Buffer: #%u (vertices: %u)",
                mr.Mesh.VertexBuffer.id, mr.Mesh.VertexCount
            );
            if (mr.Mesh.IsIndexed()) {
                ImGui::Text(
                    "Index Buffer:  #%u (indices: %u)",
                    mr.Mesh.IndexBuffer.id, mr.Mesh.IndexCount
                );
            }
            ImGui::Unindent();

            ImGui::Spacing();

            ImGui::TextUnformatted("Material");
            ImGui::Indent();
            ImGui::Text("Pipeline: #%u", mr.Material.Pipeline.id);
            ImGui::Text("Albedo:   #%u", mr.Material.Albedo.id);
            ImGui::Text("Sampler:  #%u", mr.Material.Sampler.id);
            ImGui::Unindent();

            ImGui::TextDisabled("(read-only — asset system pending)");
        }

        // ── Add Component popup ──────────────────────────────────────

        void DrawAddComponentMenu(Scene::Entity entity) {
            ImGui::Separator();

            const float buttonW = ImGui::GetContentRegionAvail().x;
            if (ImGui::Button("+ Add Component", ImVec2(buttonW, 0))) {
                ImGui::OpenPopup("AddComponentMenu");
            }

            if (ImGui::BeginPopup("AddComponentMenu")) {
                AddComponentMenuItem<Components::Name>(entity, "Name");
                AddComponentMenuItem<Components::Transform>(entity, "Transform");
                AddComponentMenuItem<Components::Camera>(entity, "Camera");
                AddComponentMenuItem<Components::Velocity>(entity, "Velocity");
                AddComponentMenuItem<Components::Tag>(entity, "Tag");
                // Components::MeshRenderer: omitted — needs asset system.
                // Components::EditorCamera: never listed (editor-only).
                ImGui::EndPopup();
            }
        }
    }

    // ── Panel ────────────────────────────────────────────────────────

    void PropertiesPanel::Draw(EditorContext& context) {
        ImGui::Begin("Properties", &m_Open);

        const auto entity = context.SelectedEntity;
        if (!entity) {
            ImGui::TextDisabled("No entity selected");
            ImGui::End();
            return;
        }

        DrawNameEditor(entity);
        ImGui::Separator();

        DrawComponent<Components::Transform>(entity, "Transform", DrawTransformBody);
        DrawComponent<Components::Camera>(entity, "Camera", DrawCameraBody);
        DrawComponent<Components::Velocity>(entity, "Velocity", DrawVelocityBody);
        DrawComponent<Components::Tag>(entity, "Tag", DrawTagBody);
        DrawComponent<Components::MeshRenderer>(entity, "Mesh Renderer", DrawMeshRendererBody);

        DrawAddComponentMenu(entity);

        ImGui::End();
    }
} // Aether
