//
// Created by ThewyRogue99 on 03.05.2026.
//

#pragma once

#include <string_view>

#include <Aether/Core/Core.h>
#include <Aether/Renderer/Pipeline.h>

namespace Aether::Renderer {
    class AETHER_API PipelineLoader {
    public:
        static PipelineHandle Load(Engine::StringView pipelinePath, Engine::StringView basePath = {});
    };
} // namespace Aether::Renderer
