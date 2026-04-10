//
// Created by ThewyRogue99 on 3.02.2026.
//

#pragma once

#include <Aether/Core/Core.h>
#include <Aether/Core/RuntimeObject.h>
#include <Aether/ECS/Registry.h>
#include <Aether/Scene/Entity.h>

namespace Aether::Systems {
    class ISystem;
}

namespace Aether::Scene {
    class AETHER_API Scene : public Engine::RuntimeObject<Scene> {
    public:
        Scene();
        ~Scene() override;

        virtual void OnLoad();
        virtual void OnUnload();

        Entity CreateEntity();
        void DestroyEntity(const Entity& entity);

        void OnUpdate(float deltaTime);

        ECS::Registry& GetRegistry();
        [[nodiscard]] const ECS::Registry& GetRegistry() const;

        template<typename... Components>
        auto View() { return m_Registry.View<Components...>(); }

        template<typename... Components>
        auto View() const { return m_Registry.View<Components...>(); }

        template<typename T, typename... Args>
        T& AddSystem(int priority, Args&&... args) {
            static_assert(std::is_base_of_v<Systems::ISystem, T>, "T must inherit from ISystem");

            auto system = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *system;

            m_Systems.push_back({ priority, std::move(system) });

            std::sort(m_Systems.begin(), m_Systems.end(),
                [](const SystemEntry& a, const SystemEntry& b) {
                    return a.priority < b.priority;
                });

            return ref;
        }

        // Convenience overload — priority defaults to 0.
        template<typename T, typename... Args>
        T& AddSystem(Args&&... args) {
            return AddSystem<T>(0, std::forward<Args>(args)...);
        }

    private:
        struct SystemEntry {
            int priority;
            std::unique_ptr<Systems::ISystem> system;
        };

        ECS::Registry m_Registry;
        std::vector<SystemEntry> m_Systems;
    };
}
