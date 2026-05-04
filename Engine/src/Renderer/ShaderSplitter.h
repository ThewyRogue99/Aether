//
// Created by Aether on 03.05.2026.
//

#pragma once

#include <string>
#include <string_view>

namespace Aether::Renderer {
    struct SplitShader {
        std::string vertexSource;
        std::string fragmentSource;
    };

    inline SplitShader SplitShaderSource(std::string_view source) {
        constexpr std::string_view kVertex = "#pragma vertex";
        constexpr std::string_view kFragment = "#pragma fragment";

        std::string preamble;
        std::string vertexBlock;
        std::string fragmentBlock;

        enum class Stage { Preamble, Vertex, Fragment } stage = Stage::Preamble;

        size_t pos = 0;
        while (pos < source.size()) {
            const size_t lineEnd = source.find('\n', pos);
            const size_t end = (lineEnd == std::string_view::npos) ? source.size() : lineEnd;

            std::string_view line = source.substr(pos, end - pos);

            // Strip trailing \r (Windows line endings)
            std::string_view raw = line;
            if (!raw.empty() && raw.back() == '\r')
                raw = raw.substr(0, raw.size() - 1);

            // Strip leading whitespace for marker comparison only
            std::string_view stripped = raw;
            while (!stripped.empty() && (stripped.front() == ' ' || stripped.front() == '\t'))
                stripped = stripped.substr(1);

            if (stripped == kVertex) {
                stage = Stage::Vertex;
            } else if (stripped == kFragment) {
                stage = Stage::Fragment;
            } else {
                std::string lineStr(line);
                lineStr += '\n';
                switch (stage) {
                    case Stage::Preamble: preamble += lineStr; break;
                    case Stage::Vertex: vertexBlock += lineStr; break;
                    case Stage::Fragment: fragmentBlock += lineStr; break;
                }
            }

            pos = (lineEnd == std::string_view::npos) ? source.size() : lineEnd + 1;
        }

        return {
            preamble + vertexBlock,
            preamble + fragmentBlock
        };
    }
} // namespace Aether::Renderer
