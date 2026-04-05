//
// Created by ThewyRogue99 on 5.04.2026.
//

#include <Aether/Scene/Entity.h>

#include <Aether/Scene/Scene.h>

namespace Aether::Scene {
    bool Entity::IsValid() const {
        return m_Handle.Value != 0 && m_Scene != nullptr && GetRegistry().IsAlive(m_Handle);
    }

    ECS::Registry& Entity::GetRegistry() {
        return m_Scene->GetRegistry();
    }

    const ECS::Registry& Entity::GetRegistry() const {
        return m_Scene->GetRegistry();
    }
}
