//
// Created by ThewyRogue99 on 5.04.2026.
//

#include <Aether/Scene/Scene.h>

#include <Aether/Components/Core/Name.h>
#include <Aether/Components/Core/Tag.h>
#include <Aether/Systems/ISystem.h>

namespace Aether::Scene {
    Scene::Scene()  = default;
    Scene::~Scene() = default;

    void Scene::OnLoad() { }

    void Scene::OnUnload() { }

    Entity Scene::CreateEntity() {
        Entity entity{ m_Registry.CreateEntity(), this };
        entity.AddComponent<Components::Name>();
        entity.AddComponent<Components::Tag>();
        return entity;
    }

    Entity Scene::CreateEntity(const Engine::String& name) {
        Entity entity{ m_Registry.CreateEntity(), this };
        entity.AddComponent<Components::Name>(Engine::String(name));
        entity.AddComponent<Components::Tag>();
        return entity;
    }

    Entity Scene::FindEntityByName(const Engine::String& name) {
        auto view = m_Registry.View<Components::Name>();
        for (const auto& [entity, nameComp] : view.each()) {
            if (nameComp.Value == name) return { entity, this };
        }
        return { };
    }

    std::vector<Entity> Scene::FindEntitiesByTag(const Engine::String& tag) {
        std::vector<Entity> result;
        auto view = m_Registry.View<Components::Tag>();
        for (const auto& [entity, tagComp] : view.each()) {
            if (tagComp.HasTag(tag)) result.emplace_back( entity, this );
        }
        return result;
    }

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
