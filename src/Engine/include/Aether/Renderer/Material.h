//
// Created by ThewyRogue99 on 21.01.2026.
//

#pragma once

#include <Aether/Math/Vector.h>
#include <Aether/Renderer/Texture.h>
#include <Aether/Renderer/Sampler.h>
#include <Aether/Renderer/Pipeline.h>

namespace Aether::Renderer {
    struct Material {
        PipelineHandle Pipeline;
        UniformBufferHandle UBO;

        TextureHandle Albedo;
        SamplerHandle Sampler;
    };
}
