//
// Created by ThewyRogue99 on 5.01.2026.
//

#include <Aether/Core/EntryPoint.h>

#include <Aether/Log/Log.h>
#include <Aether/Log/StdColorSink.h>
#include <Aether/Core/Application.h>

#include "SandboxLayer.h"
#include "Aether/Math/Math.h"
#include "Aether/Renderer/Buffer.h"
#include "Aether/Renderer/Renderer.h"
#include "Aether/Scene/Camera.h"

struct Vertex {
    float position[3];
    float normal[3];
    float texCoord[2];
};

Vertex vertices[3] = {
    {{  0.0f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.5f, 1.0f }},
    {{  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }},
    {{ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }}
};

const char* vertexSrc = R"(#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

layout(std140) uniform Camera {
    mat4 u_ViewProjection;
};

layout(std140) uniform Object {
    mat4 u_Model;
};

out vec3 v_Normal;
out vec2 v_TexCoord;

void main() {
    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    v_Normal = normalize(normalMatrix * a_Normal);

    v_TexCoord = a_TexCoord;

    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}
)";

const char* fragmentSrc = R"(#version 330 core

layout(std140) uniform Material {
    vec4 u_BaseColor;
};

const int MaxLights = 4;

struct DirectionalLight {
    vec4 direction;
    vec4 color;
};

layout(std140) uniform Light {
    DirectionalLight u_Lights[MaxLights];
    int u_LightCount;
    vec3 _pad;
};

uniform sampler2D u_Albedo;

in vec3 v_Normal;
in vec2 v_TexCoord;

out vec4 FragColor;

void main() {
    vec3 N = normalize(v_Normal);

    vec3 lighting = vec3(0.0);
    for (int i = 0; i < u_LightCount; ++i) {
        vec3 L = -normalize(u_Lights[i].direction.xyz);
        float ndotl = max(dot(N, L), 0.0);
        lighting += u_Lights[i].color.rgb * ndotl * u_Lights[i].color.a;
    }

    vec4 tex = texture(u_Albedo, v_TexCoord);
    vec3 lit = tex.rgb * u_BaseColor.rgb * lighting;

    FragColor = vec4(lit, tex.a * u_BaseColor.a);
}
)";

class SandboxApp : public Aether::Engine::Application {
public:
    SandboxApp(int argc, char** argv) : Application("Aether Sandbox", argc, argv),
        m_LogSink(std::make_unique<Aether::Log::StdColorSink>()) { }
    ~SandboxApp() override = default;

    void OnInit() override {
        Aether::Log::SetLogger(std::make_unique<Aether::Log::Logger>("Sandbox"));

        auto& logger = Aether::Log::GetLogger();
        logger.AddSink(*m_LogSink);
        logger.SetLevel(Aether::Log::LogLevel::Trace);

        PushLayer(Aether::Engine::MakeScope<SandboxLayer>());

        const auto vertexBuffer = Renderer::Renderer::CreateBuffer({
            .usage  = Renderer::BufferUsage::Vertex,
            .access = Renderer::BufferAccess::Static,
            .size   = sizeof(vertices)
        }, vertices);

        const auto shader = Renderer::Renderer::CreateShader({
            .debugName      = "TriangleShader",
            .vertexSource   = vertexSrc,
            .fragmentSource = fragmentSrc
        });

        Renderer::VertexAttribute attributes[] = {
            {
                .location   = 0,
                .format     = Renderer::VertexFormat::Float3,
                .offset     = offsetof(Vertex, position),
                .normalized = false
            },
            {
                .location   = 1,
                .format     = Renderer::VertexFormat::Float3,
                .offset     = offsetof(Vertex, normal),
                .normalized = false
            },
            {
                .location   = 2,
                .format     = Renderer::VertexFormat::Float2,
                .offset     = offsetof(Vertex, texCoord),
                .normalized = false
            }
        };

        Renderer::PipelineLayoutDesc layoutDesc;
        layoutDesc.vertexLayout = {
            .attributes     = attributes,
            .attributeCount = 3,
            .stride         = sizeof(Vertex)
        };

        layoutDesc.uniformBufferLayout = {
            { "Camera", 0, Renderer::ShaderStage::Vertex, Renderer::UniformScope::Global },
            { "Object", 1, Renderer::ShaderStage::Vertex, Renderer::UniformScope::Object },
            { "Material", 2, Renderer::ShaderStage::Fragment, Renderer::UniformScope::Material },
            { "Light", 3, Renderer::ShaderStage::Fragment, Renderer::UniformScope::Global }
        };

        const auto pipeline = Renderer::Renderer::CreatePipeline({
            .shader    = shader,
            .layout    = layoutDesc,
            .cull      = Renderer::CullMode::None,
            .depth     = Renderer::DepthTest::Disabled,
            .blending  = false,
            .debugName = "TrianglePipeline"
        });

        Engine::Camera camera;
        camera.SetPerspective(
            Math::Radians(60.f),
            16.f / 9.f,
            0.1f,
            100.f
        );

        Renderer::Renderer::SetCamera({ camera.GetViewProjection() });

        m_TexMaterial = Renderer::Renderer::CreateMaterial(pipeline, "TexMat");
        m_GreenMaterial = Renderer::Renderer::CreateMaterial(pipeline, "GreenMat");

        Renderer::Renderer::SetMaterialColor(m_GreenMaterial, {0,1,0,1});

        uint8_t pixels[] = {
            // R, G, B, A
            255, 255, 255, 255,
            0,   0,   0,   255,
            0,   0,   0,   255,
            255, 255, 255, 255
        };

        Renderer::TextureDesc td{};
        td.width = 2;
        td.height = 2;
        td.format = Renderer::TextureFormat::RGBA8;
        td.data = pixels;

        const auto tex = Renderer::Renderer::CreateTexture2D(td);

        Renderer::SamplerDesc sd{};
        sd.min = Renderer::Filter::Nearest;
        sd.mag = Renderer::Filter::Nearest;

        const auto samp = Renderer::Renderer::CreateSampler(sd);

        m_TexMaterial.Albedo = tex;
        m_TexMaterial.Sampler = samp;

        m_Triangle.VertexBuffer = vertexBuffer;
        m_Triangle.VertexCount = 3;
    }

    void OnUpdate(float DeltaTime) override {
        Renderer::Renderer::BeginFrame();

        m_LightRotation += DeltaTime;

        // Light 1: rotating white light
        Math::Vector3f lightDir1 = {
            std::cos(m_LightRotation),
            -1.0f,
            std::sin(m_LightRotation)
        };

        // Light 2: static red light from the left
        Math::Vector3f lightDir2 = {
            -1.0f,
            -0.5f,
            -0.5f
        };

        // Light 3: static blue light from the right
        Math::Vector3f lightDir3 = {
            1.0f,
            -0.5f,
            -0.5f
        };

        Renderer::Renderer::SubmitDirectionalLight(
            lightDir1,
            { 1.0f, 1.0f, 1.0f }, // white
            0.8f
        );

        Renderer::Renderer::SubmitDirectionalLight(
            lightDir2,
            { 1.0f, 0.0f, 0.0f }, // red
            0.6f
        );

        Renderer::Renderer::SubmitDirectionalLight(
            lightDir3,
            { 0.0f, 0.0f, 1.0f }, // blue
            0.6f
        );

        Renderer::Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        Renderer::Renderer::Clear();

        Renderer::Renderer::DrawMesh(m_Triangle, m_TexMaterial, Math::Translate(Math::Matrix4f::Identity(), { -1.f, 0, 0 }));
        Renderer::Renderer::DrawMesh(m_Triangle, m_GreenMaterial, Math::Translate(Math::Matrix4f::Identity(), { 1.f, 0, 0 }));

        Renderer::Renderer::EndFrame();
    }

private:
    std::unique_ptr<Log::Sink> m_LogSink;
    float m_LightRotation = 0.0f;

    Renderer::Mesh m_Triangle{};
    Renderer::Material m_TexMaterial{};
    Renderer::Material m_GreenMaterial{};
};

namespace Aether::Engine {
    Application* CreateApplication(int argc, char** argv) {
        return new SandboxApp(argc, argv);
    }
}