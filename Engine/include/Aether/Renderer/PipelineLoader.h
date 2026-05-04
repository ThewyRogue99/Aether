//
// Created by Aether on 03.05.2026.
//

#pragma once

#include <string_view>

#include <Aether/Core/Core.h>
#include <Aether/Renderer/Pipeline.h>

namespace Aether::Renderer {
    class AETHER_API PipelineLoader {
    public:
        static PipelineHandle Load(std::string_view pipelinePath, std::string_view basePath = {});
    };
} // namespace Aether::Renderer
