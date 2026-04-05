//
// Created by ThewyRogue99 on 3.02.2026.
//

#pragma once

#include <Aether/Scene/Entity.h>
#include <Aether/ECS/Registry.h>

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

    private:
        ECS::Registry m_Registry;
    };
}
