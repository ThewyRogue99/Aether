//
// Created by ThewyRogue99 on 15.02.2026.
//

#include <Aether/ECS/Registry.h>

namespace Aether::ECS {
    Entity Registry::CreateEntity() {
        return m_EntityManager.Create();
    }

    void Registry::DestroyEntity(const Entity& entity) {
        if (!m_EntityManager.IsAlive(entity))
            return;

        for (auto& [_, storage] : m_Storages)
            storage->Remove(entity);

        m_EntityManager.Destroy(entity);
    }

    bool Registry::IsAlive(const Entity& entity) const {
        return m_EntityManager.IsAlive(entity);
    }
}
