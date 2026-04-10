//
// Created by ThewyRogue99 on 6.04.2026.
//

#pragma once

#include <Aether/Systems/ISystem.h>
#include <Aether/Renderer/RenderSurface.h>

namespace Aether::Systems {
    class RenderingSystem : public ISystem {
    public:
        RenderingSystem();
        ~RenderingSystem() override;

        void OnUpdate(Scene::Scene& scene, float DeltaTime) override;

        void SetRenderSurface(Renderer::RenderSurfaceHandle surface);
        [[nodiscard]] Renderer::RenderSurfaceHandle GetRenderSurface() const;

    private:
        Renderer::RenderSurfaceHandle m_RenderSurface;
    };
}
