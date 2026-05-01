//
// Created by ThewyRogue99 on 5.04.2026.
//

#pragma once

namespace Aether::Scene {
    class Scene;
}

namespace Aether::Systems {
    class ISystem {
    public:
        virtual ~ISystem() = default;

        virtual void OnUpdate(Scene::Scene& scene, float DeltaTime) = 0;
    };
}
