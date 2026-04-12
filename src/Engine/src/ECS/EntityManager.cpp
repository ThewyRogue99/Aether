//
// Created by ThewyRogue99 on 15.02.2026.
//

#include <Aether/ECS/EntityManager.h>

namespace Aether::ECS {
    Entity EntityManager::Create() {
        uint32_t index;

        if (!m_FreeIndices.empty()) {
            index = m_FreeIndices.back();
            m_FreeIndices.pop_back();
            m_Alive[index] = true;
        } else {
            index = static_cast<uint32_t>(m_Generations.size());
            m_Generations.push_back(1);
            m_Alive.push_back(true);
        }

        return MakeEntity(index, m_Generations[index]);
    }

    void EntityManager::Destroy(const Entity& entity) {
        const uint32_t index = EntityIndex(entity);

        if (!IsAlive(entity)) return;

        // Invalidate old handles
        ++m_Generations[index];

        m_Alive[index] = false;
        m_FreeIndices.push_back(index);
    }

    bool EntityManager::IsAlive(const Entity& entity) const {
        const uint32_t index = EntityIndex(entity);

        if (index >= m_Generations.size()) return false;

        if (!m_Alive[index]) return false;

        return EntityGeneration(entity) == m_Generations[index];
    }

    uint32_t EntityManager::Capacity() const {
        return static_cast<uint32_t>(m_Generations.size());
    }
}
