//
// Created by ThewyRogue99 on 03.05.2026.
//

#pragma once

#include <Aether/Core/String.h>
#include <Aether/Core/StringView.h>

namespace Aether::Renderer {
    struct SplitShader {
        Engine::String vertexSource;
        Engine::String fragmentSource;
    };

    inline SplitShader SplitShaderSource(Engine::StringView source) {
        const Engine::String kVertex = "#pragma vertex";
        const Engine::String kFragment = "#pragma fragment";

        Engine::String preamble;
        Engine::String vertexBlock;
        Engine::String fragmentBlock;

        enum class Stage { Preamble, Vertex, Fragment } stage = Stage::Preamble;

        size_t pos = 0;
        while (pos < source.size()) {
            const size_t lineEnd = source.find('\n', pos);
            const size_t end = (lineEnd == Engine::StringView::npos) ? source.size() : lineEnd;

            const Engine::StringView line = source.substr(pos, end - pos);

            // Strip trailing \r (Windows line endings)
            Engine::StringView raw = line;
            if (!raw.empty() && raw.back() == '\r') raw = raw.substr(0, raw.size() - 1);

            // Strip leading whitespace for marker comparison only
            Engine::StringView stripped = raw;
            while (!stripped.empty() && (stripped.front() == ' ' || stripped.front() == '\t'))
                stripped = stripped.substr(1);

            if (stripped == kVertex) {
                stage = Stage::Vertex;
            } else if (stripped == kFragment) {
                stage = Stage::Fragment;
            } else {
                Engine::String lineStr(line);
                lineStr += '\n';
                switch (stage) {
                    case Stage::Preamble: preamble += lineStr; break;
                    case Stage::Vertex: vertexBlock += lineStr; break;
                    case Stage::Fragment: fragmentBlock += lineStr; break;
                }
            }

            pos = (lineEnd == Engine::StringView::npos) ? source.size() : lineEnd + 1;
        }

        return {
            preamble + vertexBlock,
            preamble + fragmentBlock
        };
    }
} // namespace Aether::Renderer
