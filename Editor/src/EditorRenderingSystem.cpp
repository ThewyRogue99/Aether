//
// Created by ThewyRogue99 on 15.04.2026.
//

#include "EditorRenderingSystem.h"

#include <Aether/Math/Math.h>
#include <Aether/Scene/Scene.h>
#include <Aether/Components/Core/Transform.h>

#include "Components/EditorCamera.h"

namespace Aether {
    EditorRenderingSystem::EditorRenderingSystem(Renderer::RenderSurfaceHandle surface) : RenderingSystem(surface) { }

    std::optional<Renderer::CameraDesc> EditorRenderingSystem::ResolveCamera(Scene::Scene& scene, float aspect) {
        for (const auto& [
            entity,
            transform,
            camera
        ] : scene.View<Components::Transform, Components::EditorCamera>().each()) {
            const float pitch = Math::Radians(transform.Rotation.x);
            const float yaw = Math::Radians(transform.Rotation.y);

            const Math::Vector3f forward = Math::Normalize(Math::Vector3f{
                std::cos(pitch) * std::sin(yaw),
                std::sin(pitch),
                std::cos(pitch) * std::cos(yaw)
            });

            const Math::Vector3f worldUp = { 0.f, 1.f, 0.f };
            const Math::Vector3f right = Math::Normalize(Math::Cross(forward, worldUp));
            const Math::Vector3f up = Math::Normalize(Math::Cross(right, forward));

            const auto view = Math::LookAt(transform.Position, transform.Position + forward, up);

            const auto projection = Math::Perspective(
                Math::Radians(camera.FOV),
                aspect,
                camera.Near,
                camera.Far
            );

            return Renderer::CameraDesc{ projection * view };
        }

        return std::nullopt;
    }
} // Aether
