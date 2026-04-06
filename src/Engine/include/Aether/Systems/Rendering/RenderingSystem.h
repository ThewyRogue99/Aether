//
// Created by ThewyRogue99 on 6.04.2026.
//

#pragma once

#include <Aether/Systems/ISystem.h>

namespace Aether::Systems {
    class RenderingSystem : public ISystem {
    public:
        RenderingSystem();
        ~RenderingSystem() override;

        void OnUpdate(Scene::Scene& scene, float DeltaTime) override;
    };
}
