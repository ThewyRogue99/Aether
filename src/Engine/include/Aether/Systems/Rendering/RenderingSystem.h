//
// Created by ThewyRogue99 on 6.04.2026.
//

#pragma once

#include <Aether/Systems/ISystem.h>
#include <Aether/Renderer/RenderSurface.h>

namespace Aether::Systems {
    class RenderingSystem : public ISystem {
    public:
        explicit RenderingSystem(Renderer::RenderSurfaceHandle renderSurface);
        ~RenderingSystem() override;

        void OnUpdate(Scene::Scene& scene, float DeltaTime) override;

    private:
        Renderer::RenderSurfaceHandle m_RenderSurface;
    };
}
