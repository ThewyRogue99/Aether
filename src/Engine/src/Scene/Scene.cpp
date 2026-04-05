//
// Created by ThewyRogue99 on 5.04.2026.
//

#include <Aether/Scene/Scene.h>

#include <Aether/Systems/ISystem.h>

namespace Aether::Scene {
    Scene::Scene() = default;

    Scene::~Scene() = default;

    Entity Scene::CreateEntity() {
        return { m_Registry.CreateEntity(), this };
    }

    void Scene::DestroyEntity(const Entity& entity) {
        AETHER_ASSERT_MSG(entity, "Invalid entity!");
        AETHER_ASSERT_MSG(entity.GetScene() == this, "Entity does not belong to this scene!");

        m_Registry.DestroyEntity(entity.GetHandle());
    }

    void Scene::OnUpdate(float DeltaTime) {
        for (const auto& system : m_Systems) {
            system->OnUpdate(*this, DeltaTime);
        }
    }

    ECS::Registry& Scene::GetRegistry() {
        return m_Registry;
    }

    const ECS::Registry& Scene::GetRegistry() const {
        return m_Registry;
    }
}
