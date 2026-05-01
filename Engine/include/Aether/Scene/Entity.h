//
// Created by ThewyRogue99 on 15.02.2026.
//

#pragma once

#include <utility>

#include <Aether/ECS/Entity.h>
#include <Aether/ECS/Registry.h>

namespace Aether::Scene {
    class Scene;

    class Entity {
    public:
        Entity() = default;

        Entity(ECS::Entity handle, Scene* scene)
            : m_Handle(handle), m_Scene(scene) { }

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            auto& registry = GetRegistry();
            registry.AddComponent<T>(m_Handle, std::forward<Args>(args)...);

            return registry.GetComponent<T>(m_Handle);
        }

        template<typename T>
        T& GetComponent() {
            auto& registry = GetRegistry();
            return registry.GetComponent<T>(m_Handle);
        }

        template<typename T>
        const T& GetComponent() const {
            const auto& registry = GetRegistry();
            return registry.GetComponent<T>(m_Handle);
        }

        template<typename T>
        [[nodiscard]] bool HasComponent() const {
            const auto& registry = GetRegistry();
            return registry.HasComponent<T>(m_Handle);
        }

        template<typename T>
        void RemoveComponent() {
            auto& registry = GetRegistry();
            registry.RemoveComponent<T>(m_Handle);
        }

        [[nodiscard]] bool IsValid() const;

        operator bool() const { return IsValid(); }

        bool operator==(const Entity& other) const {
            return m_Handle == other.m_Handle && m_Scene == other.m_Scene;
        }

        bool operator!=(const Entity& other) const = default;

        [[nodiscard]] ECS::Entity GetHandle() const { return m_Handle; }

        Scene* GetScene() { return m_Scene; }
        [[nodiscard]] const Scene* GetScene() const { return m_Scene; }

    private:
        ECS::Registry& GetRegistry();
        [[nodiscard]] const ECS::Registry& GetRegistry() const;

    private:
        ECS::Entity m_Handle{};
        Scene* m_Scene = nullptr;
    };
}
