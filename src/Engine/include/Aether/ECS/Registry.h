//
// Created by ThewyRogue99 on 15.02.2026.
//

#pragma once

#include <typeindex>
#include <unordered_map>

#include <Aether/Core/Assert.h>
#include <Aether/ECS/EntityManager.h>
#include <Aether/ECS/ComponentStorage.h>

namespace Aether::ECS {
    class Registry {
    public:
        Entity CreateEntity();

        void DestroyEntity(const Entity& entity);

        bool IsAlive(const Entity& entity) const;

        template<typename T>
        void AddComponent(const Entity& entity, const T& component) {
            AETHER_ASSERT_MSG(IsAlive(entity), "Invalid entity!");

            GetStorage<T>().Add(entity, component);
        }

        template<typename T>
        T& GetComponent(const Entity& entity) {
            AETHER_ASSERT_MSG(IsAlive(entity), "Invalid entity!");

            return GetStorage<T>().Get(entity);
        }

        template<typename T>
        bool HasComponent(const Entity& entity) {
            if (!IsAlive(entity)) return false;

            return GetStorage<T>().Has(entity);
        }

    private:
        template<typename T>
        ComponentStorage<T>& GetStorage() {
            const std::type_index type = typeid(T);

            if (!m_Storages.contains(type)) {
                m_Storages[type] = std::make_unique<ComponentStorage<T>>();
            }

            return *static_cast<ComponentStorage<T>*>(m_Storages[type].get());
        }

    private:
        EntityManager m_EntityManager;

        std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>> m_Storages;
    };
}
