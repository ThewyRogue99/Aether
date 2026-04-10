//
// Created by ThewyRogue99 on 6.04.2026.
//

#include <Aether/Systems/Rendering/RenderingSystem.h>

#include <Aether/Math/Math.h>
#include <Aether/Scene/Scene.h>
#include <Aether/Renderer/Renderer.h>
#include <Aether/Components/Rendering/MeshRenderer.h>

#include "Aether/Components/Core/Transform.h"
#include "Aether/Components/Rendering/Camera.h"

namespace Aether::Systems {
    RenderingSystem::RenderingSystem() = default;

    RenderingSystem::~RenderingSystem() = default;

    void RenderingSystem::OnUpdate(Scene::Scene& scene, float DeltaTime) {
        Renderer::Renderer::BeginFrame();

        Renderer::Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        Renderer::Renderer::Clear();

        for (const auto& [entity, transform, camera] : scene.View<Components::Transform, Components::Camera>().each()) {
            if (camera.Primary) {
                const auto& position = transform.Position;

                const float pitch = Math::Radians(transform.Rotation.x);
                const float yaw = Math::Radians(transform.Rotation.y);

                Math::Vector3f forward = {
                    std::cos(pitch) * std::sin(yaw),
                    std::sin(pitch),
                    std::cos(pitch) * std::cos(yaw)
                };

                forward = Math::Normalize(forward);

                Math::Vector3f worldUp = { 0.0f, 1.0f, 0.0f };
                Math::Vector3f right = Math::Normalize(Math::Cross(forward, worldUp));
                Math::Vector3f up = Math::Normalize(Math::Cross(right, forward));

                const auto view = Math::LookAt(position, position + forward, up);

                constexpr float aspect = 16.0f / 9.0f;
                const auto projection = Math::Perspective(
                    Math::Radians(camera.FOV),
                    aspect,
                    camera.Near,
                    camera.Far
                );

                Renderer::Renderer::SetCamera({ projection * view });

                break;
            }
        }

        for (const auto& [entity, transform, mesh] : scene.View<Components::Transform, Components::MeshRenderer>().each()) {
            auto model = Math::Matrix4f::Identity();

            model = Math::Translate(model, transform.Position);

            model = Math::Rotate(model, Math::Radians(transform.Rotation.x), { 1.0f, 0.0f, 0.0f });
            model = Math::Rotate(model, Math::Radians(transform.Rotation.y), { 0.0f, 1.0f, 0.0f });
            model = Math::Rotate(model, Math::Radians(transform.Rotation.z), { 0.0f, 0.0f, 1.0f });

            model = Math::Scale(model, transform.Scale);

            Renderer::Renderer::DrawMesh(mesh.Mesh, mesh.Material, model);
        }

        Renderer::Renderer::EndFrame();
    }
}
