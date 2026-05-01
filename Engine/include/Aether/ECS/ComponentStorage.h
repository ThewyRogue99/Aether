//
// Created by ThewyRogue99 on 15.02.2026.
//

#pragma once

#include <vector>
#include <limits>

#include <Aether/ECS/Entity.h>
#include <Aether/ECS/IComponentStorage.h>

namespace Aether::ECS {
    template<typename T>
    class ComponentStorage : public IComponentStorage {
        static constexpr uint32_t INVALID_INDEX = std::numeric_limits<uint32_t>::max();
    public:
        ComponentStorage() = default;
        ~ComponentStorage() override = default;

        void Add(const Entity& entity, const T& component) {
            EnsureSparseSize(entity);

            if (Has(entity))
                return;

            const uint32_t denseIndex = static_cast<uint32_t>(m_Dense.size());

            m_Dense.push_back(component);
            m_Entities.push_back(entity);

            m_Sparse[EntityIndex(entity)] = denseIndex;
        }

        void Remove(const Entity& entity) override {
            if (!Has(entity)) return;

            const uint32_t entityIndex = EntityIndex(entity);
            const uint32_t denseIndex = m_Sparse[entityIndex];
            const uint32_t lastDenseIndex = static_cast<uint32_t>(m_Dense.size() - 1);

            if (denseIndex != lastDenseIndex) {
                m_Dense[denseIndex] = std::move(m_Dense[lastDenseIndex]);
                m_Entities[denseIndex] = m_Entities[lastDenseIndex];

                const uint32_t movedEntityIndex = EntityIndex(m_Entities[denseIndex]);
                m_Sparse[movedEntityIndex] = denseIndex;
            }

            m_Dense.pop_back();
            m_Entities.pop_back();

            m_Sparse[entityIndex] = INVALID_INDEX;
        }

        T& Get(const Entity& entity) {
            return m_Dense[m_Sparse[EntityIndex(entity)]];
        }

        bool Has(const Entity& entity) const {
            const uint32_t index = EntityIndex(entity);

            if (index >= m_Sparse.size())
                return false;

            const uint32_t denseIndex = m_Sparse[index];

            if (denseIndex == INVALID_INDEX)
                return false;

            return m_Entities[denseIndex].Value == entity.Value;
        }

        size_t Size() const { return m_Dense.size(); }
        const std::vector<Entity>& Entities() const { return m_Entities; }

    private:
        void EnsureSparseSize(const Entity& entity) {
            const uint32_t index = EntityIndex(entity);

            if (index >= m_Sparse.size()) m_Sparse.resize(index + 1, INVALID_INDEX);
        }

        std::vector<T> m_Dense;
        std::vector<Entity> m_Entities;
        std::vector<uint32_t> m_Sparse;
    };
}
