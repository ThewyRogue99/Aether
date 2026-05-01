//
// Created by ThewyRogue99 on 15.04.2026.
//

#pragma once

#include <Aether/Scene/Entity.h>
#include <Aether/Renderer/RenderSurface.h>

namespace Aether::Scene {
    class Scene;
}

namespace Aether {
    struct EditorContext {
        Scene::Scene* ActiveScene = nullptr;
        Scene::Entity SelectedEntity;
        Renderer::RenderSurfaceHandle ViewportSurface;
    };
} // Aether
