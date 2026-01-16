//
// Created by ThewyRogue99 on 5.01.2026.
//

#include <Aether/Core/EntryPoint.h>

#include <Aether/Log/Log.h>
#include <Aether/Log/StdColorSink.h>
#include <Aether/Core/Application.h>

#include "SandboxLayer.h"
#include "Aether/Renderer/Buffer.h"
#include "Aether/Renderer/Renderer.h"

struct Vertex {
    float position[3];
};

Vertex vertices[3] = {
    {{  0.0f,  0.5f, 0.0f }},
    {{  0.5f, -0.5f, 0.0f }},
    {{ -0.5f, -0.5f, 0.0f }}
};

const char* vertexSrc = R"(#version 330 core
layout(location = 0) in vec3 a_Position;

void main()
{
    gl_Position = vec4(a_Position, 1.0);
}
)";

const char* fragmentSrc = R"(#version 330 core
out vec4 o_Color;

void main()
{
    o_Color = vec4(1.0, 0.0, 0.0, 1.0); // red
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

        m_VertexBuffer = Renderer::Renderer::CreateBuffer({
            .usage  = Renderer::BufferUsage::Vertex,
            .access = Renderer::BufferAccess::Static,
            .size   = sizeof(vertices)
        }, vertices);

        Renderer::ShaderHandle shader = Renderer::Renderer::CreateShader({
            .debugName      = "TriangleShader",
            .vertexSource   = vertexSrc,
            .fragmentSource = fragmentSrc
        });

        Renderer::VertexAttribute attributes[] = {{
            .location   = 0,
            .format     = Renderer::VertexFormat::Float3,
            .offset     = 0,
            .normalized = false
        }};

        Renderer::VertexLayout layout{
            .attributes     = attributes,
            .attributeCount = 1,
            .stride         = sizeof(Vertex)
        };

        m_Pipeline = Renderer::Renderer::CreatePipeline({
            .shader    = shader,
            .layout    = layout,
            .cull      = Renderer::CullMode::None,
            .depth     = Renderer::DepthTest::Disabled,
            .blending  = false,
            .debugName = "TrianglePipeline"
        });
    }

    void OnUpdate(float DeltaTime) override {
        Renderer::Renderer::BeginFrame();

        Renderer::Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        Renderer::Renderer::Clear();

        Renderer::Renderer::BindPipeline(m_Pipeline);
        Renderer::Renderer::BindVertexBuffer(m_VertexBuffer);
        Renderer::Renderer::Draw(3, 0);

        Renderer::Renderer::EndFrame();
    }

private:
    std::unique_ptr<Log::Sink> m_LogSink;

    Renderer::PipelineHandle m_Pipeline;
    Renderer::BufferHandle m_VertexBuffer;
};

namespace Aether::Engine {
    Application* CreateApplication(int argc, char** argv) {
        return new SandboxApp(argc, argv);
    }
}