//
// Created by ThewyRogue99 on 5.04.2026.
//

#include <Aether/Systems/Movement/MovementSystem.h>

#include <Aether/Scene/Scene.h>
#include <Aether/Components/Core/Transform.h>
#include <Aether/Components/Physics/Velocity.h>

namespace Aether::Systems {
    MovementSystem::MovementSystem() = default;

    MovementSystem::~MovementSystem() = default;

    void MovementSystem::OnUpdate(Scene::Scene& scene, float DeltaTime) {
        const auto view = scene.View<Components::Transform, Components::Velocity>();
        auto& registry = scene.GetRegistry();

        for (auto entity : view) {
            auto& transform = registry.GetComponent<Components::Transform>(entity);
            auto& velocity  = registry.GetComponent<Components::Velocity>(entity);

            transform.Position += velocity.Value * DeltaTime;
        }
    }
}
