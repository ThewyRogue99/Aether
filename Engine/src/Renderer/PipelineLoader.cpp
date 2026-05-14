//
// Created by ThewyRogue99 on 03.05.2026.
//

#include <Aether/Renderer/PipelineLoader.h>

#include "ShaderSplitter.h"

#include <fstream>
#include <sstream>
#include <vector>

#include <Aether/Core/Json.h>
#include <Aether/Core/String.h>
#include <Aether/Log/Log.h>
#include <Aether/Renderer/Renderer.h>
#include <Aether/Renderer/VertexLayout.h>

namespace Aether::Renderer {
    static Engine::String DirOf(Engine::StringView path) {
        const uint32_t a = path.rfind('/');
        const uint32_t b = path.rfind('\\');

        // Pick the rightmost separator that was actually found.
        uint32_t pos = Engine::StringView::npos;
        if (a != Engine::StringView::npos) pos = a;
        if (b != Engine::StringView::npos && (pos == Engine::StringView::npos || b > pos)) pos = b;

        if (pos == Engine::StringView::npos) return {};
        return Engine::String(path.substr(0, pos + 1));
    }

    static Engine::String ReadTextFile(const Engine::String& path) {
        std::ifstream file(path.data());
        if (!file.is_open()) {
            AETHER_ERROR("PipelineLoader: cannot open '%s'", path.c_str());
            return {};
        }
        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str().c_str();
    }

    static VertexFormat ParseVertexFormat(Engine::StringView s) {
        if (s == "Float") return VertexFormat::Float;
        if (s == "Float2") return VertexFormat::Float2;
        if (s == "Float3") return VertexFormat::Float3;
        if (s == "Float4") return VertexFormat::Float4;

        AETHER_WARN(
            "PipelineLoader: unknown vertex format '%.*s', defaulting to Float3",
            static_cast<int>(s.size()), s.data()
        );
        return VertexFormat::Float3;
    }

    static CullMode ParseCullMode(Engine::StringView s) {
        if (s == "None") return CullMode::None;
        if (s == "Back") return CullMode::Back;
        if (s == "Front") return CullMode::Front;

        AETHER_WARN(
            "PipelineLoader: unknown cull mode '%.*s', defaulting to Back",
            static_cast<int>(s.size()),
            s.data()
        );
        return CullMode::Back;
    }

    static DepthTest ParseDepthTest(Engine::StringView s) {
        if (s == "Disabled") return DepthTest::Disabled;
        if (s == "LessEqual") return DepthTest::LessEqual;

        AETHER_WARN(
            "PipelineLoader: unknown depth test '%.*s', defaulting to Disabled",
            static_cast<int>(s.size()),
            s.data()
        );
        return DepthTest::Disabled;
    }

    static ShaderStage ParseShaderStages(const Engine::JsonValue& arr) {
        uint8_t mask = 0;
        arr.ForEach([&](const Engine::JsonValue& v) {
            const Engine::String s = v.AsString();
            if (s == "Vertex") mask |= static_cast<uint8_t>(ShaderStage::Vertex);
            else if (s == "Fragment") mask |= static_cast<uint8_t>(ShaderStage::Fragment);
            else if (s == "Compute") mask |= static_cast<uint8_t>(ShaderStage::Compute);
            else AETHER_WARN("PipelineLoader: unknown shader stage '%s'", s.c_str());
        });
        return static_cast<ShaderStage>(mask);
    }

    static UniformScope ParseUniformScope(Engine::StringView s) {
        if (s == "Global") return UniformScope::Global;
        if (s == "Scene") return UniformScope::Scene;
        if (s == "Object") return UniformScope::Object;
        if (s == "Material") return UniformScope::Material;

        AETHER_WARN(
            "PipelineLoader: unknown uniform scope '%.*s', defaulting to Global",
            static_cast<int>(s.size()),
            s.data()
        );
        return UniformScope::Global;
    }

    PipelineHandle PipelineLoader::Load(Engine::StringView pipelinePath, Engine::StringView basePath) {
        const auto root = Engine::JsonValue::ParseFile(pipelinePath);
        if (root.IsNull()) {
            AETHER_ERROR(
                "PipelineLoader: failed to parse '%.*s'",
                static_cast<int>(pipelinePath.size()),
                pipelinePath.data()
            );
            return {};
        }

        Engine::String apiKey;
        switch (Renderer::GetAPI()) {
            case RenderAPI::OpenGL: apiKey = "opengl"; break;
            case RenderAPI::Vulkan: apiKey = "vulkan"; break;
            default:
                AETHER_ERROR("PipelineLoader: unsupported render API");
                return {};
        }

        const auto shadersNode = root.At("shaders");
        if (shadersNode.IsNull() || !shadersNode.Contains(apiKey)) {
            AETHER_ERROR(
                "PipelineLoader: no shader entry for api '%s' in '%.*s'",
                apiKey.c_str(),
                static_cast<int>(pipelinePath.size()), pipelinePath.data()
            );
            return {};
        }

        const Engine::String base = basePath.empty() ? DirOf(pipelinePath) : Engine::String(basePath);
        const Engine::String shaderRel = shadersNode.At(apiKey).AsString();
        const Engine::String shaderPath = base + shaderRel;

        const Engine::String shaderSrc = ReadTextFile(shaderPath);
        if (shaderSrc.empty()) return {};

        const auto [vertSrc, fragSrc] = SplitShaderSource(shaderSrc);

        const auto shader = Renderer::CreateShader({
            .debugName = shaderPath.c_str(),
            .vertexSource = vertSrc.c_str(),
            .fragmentSource = fragSrc.c_str()
        });
        if (!shader) {
            AETHER_ERROR("PipelineLoader: shader compilation failed for '%s'", shaderPath.c_str());
            return {};
        }

        const auto layoutNode = root.At("vertex_layout");
        const auto stride = static_cast<uint32_t>(layoutNode.At("stride").AsInt(0));

        std::vector<VertexAttribute> attributes;
        layoutNode.At("attributes").ForEach([&](const Engine::JsonValue& attr) {
            VertexAttribute a;
            a.location = static_cast<uint32_t>(attr.At("location").AsInt(0));
            a.format = ParseVertexFormat(attr.At("format").AsString());
            a.offset = static_cast<uint32_t>(attr.At("offset").AsInt(0));
            a.normalized = attr.At("normalized").AsBool(false);
            attributes.push_back(a);
        });

        std::vector<UniformBufferLayoutDesc> ubos;
        root.At("uniform_buffers").ForEach([&](const Engine::JsonValue& ubo) {
            UniformBufferLayoutDesc desc;
            const auto nameStr = ubo.At("name").AsString();
            desc.Name = Engine::String(nameStr.c_str());
            desc.Binding = static_cast<uint32_t>(ubo.At("binding").AsInt(0));
            desc.Stages = ParseShaderStages(ubo.At("stages"));
            desc.Scope = ParseUniformScope(ubo.At("scope").AsString());
            ubos.push_back(std::move(desc));
        });

        PipelineLayoutDesc layoutDesc;
        layoutDesc.vertexLayout = {
            .attributes = attributes.data(),
            .attributeCount = static_cast<uint32_t>(attributes.size()),
            .stride = stride
        };
        layoutDesc.uniformBufferLayout = std::move(ubos);

        PipelineDesc pipelineDesc;
        pipelineDesc.shader = shader;
        pipelineDesc.layout = std::move(layoutDesc);
        pipelineDesc.cull = ParseCullMode (root.At("cull").AsString("Back"));
        pipelineDesc.depth = ParseDepthTest(root.At("depth").AsString("Disabled"));
        pipelineDesc.blending = root.At("blending").AsBool(false);
        pipelineDesc.debugName = shaderPath.c_str();

        return Renderer::CreatePipeline(pipelineDesc);
    }
} // namespace Aether::Renderer
