//
// Created by ThewyRogue99 on 5.04.2026.
//

#include <Aether/Scene/Scene.h>

#include <Aether/Systems/ISystem.h>

namespace Aether::Scene {
    Scene::Scene()  = default;
    Scene::~Scene() = default;

    void Scene::OnLoad() { }

    void Scene::OnUnload() { }

    Entity Scene::CreateEntity() { return { m_Registry.CreateEntity(), this }; }

    void Scene::DestroyEntity(const Entity& entity) {
        AETHER_ASSERT_MSG(entity, "Scene::DestroyEntity — invalid entity");
        AETHER_ASSERT_MSG(entity.GetScene() == this, "Scene::DestroyEntity — entity belongs to a different scene");

        m_Registry.DestroyEntity(entity.GetHandle());
    }

    void Scene::OnUpdate(float deltaTime) {
        for (const auto& entry : m_Systems) entry.system->OnUpdate(*this, deltaTime);
    }

    ECS::Registry& Scene::GetRegistry() {
        return m_Registry;
    }

    const ECS::Registry& Scene::GetRegistry() const {
        return m_Registry;
    }
}
