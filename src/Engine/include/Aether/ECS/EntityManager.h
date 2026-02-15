//
// Created by ThewyRogue99 on 15.02.2026.
//

#pragma once

#include <vector>

#include <Aether/ECS/Entity.h>

namespace Aether::ECS {
    class EntityManager {
    public:
        Entity Create();

        void Destroy(const Entity& entity);

        [[nodiscard]] bool IsAlive(const Entity& entity) const;

        [[nodiscard]] uint32_t Capacity() const;

    private:
        std::vector<uint32_t> m_Generations;
        std::vector<uint32_t> m_FreeIndices;
        std::vector<bool> m_Alive;
    };
}
