//
// Created by Aether on 03.05.2026.
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
    static std::string DirOf(std::string_view path) {
        const auto pos = path.find_last_of("/\\");
        if (pos == std::string_view::npos) return {};
        return std::string(path.substr(0, pos + 1));
    }

    static std::string ReadTextFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            AETHER_ERROR("PipelineLoader: cannot open '%s'", path.c_str());
            return {};
        }
        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    static VertexFormat ParseVertexFormat(std::string_view s) {
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

    static CullMode ParseCullMode(std::string_view s) {
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

    static DepthTest ParseDepthTest(std::string_view s) {
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
            const std::string s = v.AsString();
            if (s == "Vertex") mask |= static_cast<uint8_t>(ShaderStage::Vertex);
            else if (s == "Fragment") mask |= static_cast<uint8_t>(ShaderStage::Fragment);
            else if (s == "Compute") mask |= static_cast<uint8_t>(ShaderStage::Compute);
            else AETHER_WARN("PipelineLoader: unknown shader stage '%s'", s.c_str());
        });
        return static_cast<ShaderStage>(mask);
    }

    static UniformScope ParseUniformScope(std::string_view s) {
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

    PipelineHandle PipelineLoader::Load(std::string_view pipelinePath, std::string_view basePath) {
        const auto root = Engine::JsonValue::ParseFile(pipelinePath);
        if (root.IsNull()) {
            AETHER_ERROR(
                "PipelineLoader: failed to parse '%.*s'",
                static_cast<int>(pipelinePath.size()),
                pipelinePath.data()
            );
            return {};
        }

        std::string apiKey;
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

        const std::string base = basePath.empty() ? DirOf(pipelinePath) : std::string(basePath);
        const std::string shaderRel = shadersNode.At(apiKey).AsString();
        const std::string shaderPath = base + shaderRel;

        const std::string shaderSrc = ReadTextFile(shaderPath);
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
