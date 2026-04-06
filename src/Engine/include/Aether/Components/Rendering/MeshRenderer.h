//
// Created by ThewyRogue99 on 6.04.2026.
//

#pragma once

#include <Aether/Renderer/Mesh.h>
#include <Aether/Renderer/Material.h>

namespace Aether::Components {
    struct MeshRenderer {
        Renderer::Mesh Mesh{};
        Renderer::Material Material{};
    };
}
