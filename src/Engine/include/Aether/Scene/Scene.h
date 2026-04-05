//
// Created by ThewyRogue99 on 3.02.2026.
//

#pragma once

#include <Aether/ECS/Registry.h>
#include <Aether/Scene/Entity.h>

namespace Aether::Systems {
    class ISystem;
}

namespace Aether::Scene {
    class Scene {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity();
        void DestroyEntity(const Entity& entity);

        void OnUpdate(float DeltaTime);

        ECS::Registry& GetRegistry();
        [[nodiscard]] const ECS::Registry& GetRegistry() const;

        template<typename... Components>
        auto View() {
            return m_Registry.View<Components...>();
        }

        template<typename... Components>
        auto View() const {
            return m_Registry.View<Components...>();
        }

        template<typename T, typename... Args>
        T& AddSystem(Args&&... args) {
            static_assert(std::is_base_of_v<Systems::ISystem, T>, "T must inherit from ISystem");

            auto system = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *system;

            m_Systems.emplace_back(std::move(system));
            return ref;
        }

    private:
        ECS::Registry m_Registry;
        std::vector<std::unique_ptr<Systems::ISystem>> m_Systems;
    };
}
