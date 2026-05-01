//
// Created by ThewyRogue99 on 15.02.2026.
//

#pragma once

#include <Aether/ECS/Entity.h>

namespace Aether::ECS {
    class IComponentStorage {
    public:
        virtual ~IComponentStorage() = default;

        virtual void Remove(const Entity& entity) = 0;
    };
}
