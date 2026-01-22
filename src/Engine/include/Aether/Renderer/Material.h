//
// Created by ThewyRogue99 on 21.01.2026.
//

#pragma once

#include <Aether/Renderer/Pipeline.h>
#include <Aether/Math/Vector.h>

namespace Aether::Renderer {
    struct MaterialData {
        Math::Vector4f BaseColor = Math::Vector4f(1.f);
    };

    struct Material {
        PipelineHandle Pipeline;
        UniformBufferHandle UBO;
        MaterialData Data;
    };
}