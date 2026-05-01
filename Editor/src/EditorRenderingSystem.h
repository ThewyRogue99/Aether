//
// Created by ThewyRogue99 on 15.04.2026.
//

#pragma once

#include <Aether/Systems/Rendering/RenderingSystem.h>

namespace Aether {
    class EditorRenderingSystem : public Systems::RenderingSystem {
    public:
        explicit EditorRenderingSystem(Renderer::RenderSurfaceHandle surface);

    protected:
        std::optional<Renderer::CameraDesc> ResolveCamera(Scene::Scene& scene, float aspect) override;
    };
} // Aether
