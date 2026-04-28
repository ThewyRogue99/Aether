//
// Created by ThewyRogue99 on 6.04.2026.
//

#pragma once

#include <optional>

#include <Aether/Systems/ISystem.h>
#include <Aether/Renderer/Renderer.h>
#include <Aether/Renderer/RenderSurface.h>

namespace Aether::Systems {
    class RenderingSystem : public ISystem {
    public:
        explicit RenderingSystem(Renderer::RenderSurfaceHandle renderSurface);
        ~RenderingSystem() override;

        void OnUpdate(Scene::Scene& scene, float DeltaTime) override;

    protected:
        // Resolve the camera to render with this frame.
        // Default: searches the scene for the first Primary Camera component.
        // Subclasses may override to provide an editor camera, replay camera, etc.
        virtual std::optional<Renderer::CameraDesc> ResolveCamera(
            Scene::Scene& scene, float aspect);

        Renderer::RenderSurfaceHandle m_RenderSurface;
    };
}
