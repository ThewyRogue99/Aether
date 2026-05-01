//
// Created by ThewyRogue99 on 5.04.2026.
//

#pragma once

#include <Aether/Systems/ISystem.h>

namespace Aether::Systems {
    class MovementSystem : public ISystem {
    public:
        MovementSystem();
        ~MovementSystem() override;

        void OnUpdate(Scene::Scene& scene, float DeltaTime) override;
    };
}
