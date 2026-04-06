//
// Created by ThewyRogue99 on 6.04.2026.
//

#include "SandboxLayer.h"

#include "Aether/Components/Core/Transform.h"
#include "Aether/Components/Physics/Velocity.h"
#include "Aether/Components/Rendering/Camera.h"
#include "Aether/Components/Rendering/MeshRenderer.h"
#include "Aether/Core/Layer.h"
#include "Aether/Events/EventDispatcher.h"
#include "Aether/Input/Input.h"
#include "Aether/Input/KeyCode.h"
#include "Aether/Log/Log.h"
#include "Aether/Events/KeyEvent.h"
#include "Aether/Math/Math.h"
#include "Aether/Math/Vector.h"
#include "Aether/Renderer/Renderer.h"
#include "Aether/Scene/Scene.h"
#include "Aether/Scene/Entity.h"
#include "Aether/Systems/Movement/MovementSystem.h"
#include "Aether/Systems/Rendering/RenderingSystem.h"

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

    out vec2 v_TexCoord;

    void main() {
        v_TexCoord = a_TexCoord;
        gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
    }
)";

const char* fragmentSrc = R"(#version 330 core

    layout(std140) uniform Material {
        vec4 u_BaseColor;
    };

    in vec2 v_TexCoord;
    out vec4 FragColor;

    uniform sampler2D u_Albedo;

    void main() {
        vec4 tex = texture(u_Albedo, v_TexCoord);
        FragColor = tex * u_BaseColor;
    }
)";

namespace Aether {

    SandboxLayer::SandboxLayer() : Layer("Gameplay") { }

    SandboxLayer::~SandboxLayer() = default;

    void SandboxLayer::OnAttach() {
        m_ActiveScene = Engine::MakeScope<Scene::Scene>();

        m_ActiveScene->AddSystem<Systems::MovementSystem>();
        m_ActiveScene->AddSystem<Systems::RenderingSystem>();

        const auto vertexBuffer = Renderer::Renderer::Renderer::CreateBuffer({
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
            { "Material", 2, Renderer::ShaderStage::Fragment, Renderer::UniformScope::Material }
        };

        const auto pipeline = Renderer::Renderer::CreatePipeline({
            .shader    = shader,
            .layout    = layoutDesc,
            .cull      = Renderer::CullMode::None,
            .depth     = Renderer::DepthTest::Disabled,
            .blending  = false,
            .debugName = "TrianglePipeline"
        });

        // Camera
        {
            m_CameraEntity = m_ActiveScene->CreateEntity();

            auto& transform = m_CameraEntity.AddComponent<Components::Transform>();
            transform.Position = Math::Vector3f{ 0.f, 0.f, -2.f };

            auto& camera = m_CameraEntity.AddComponent<Components::Camera>();
            camera.FOV = 60.f;
            camera.Near = 0.1f;
            camera.Far = 100.f;
            camera.Primary = true;
        }

        // Green triangle
        {
            m_GreenEntity = m_ActiveScene->CreateEntity();

            auto& transform = m_GreenEntity.AddComponent<Components::Transform>();
            transform.Position = Math::Vector3f{ 0.5f, 0.f, 0.f };

            auto greenMaterial = Renderer::Renderer::CreateMaterial(pipeline, "GreenMat");

            Renderer::Renderer::SetMaterialColor(greenMaterial, {0,1,0,1});

            auto& mesh = m_GreenEntity.AddComponent<Components::MeshRenderer>();
            mesh.Material = greenMaterial;
            mesh.Mesh.VertexBuffer = vertexBuffer;
            mesh.Mesh.VertexCount = 3;
        }

        // Texture triangle
        {
            m_TexEntity = m_ActiveScene->CreateEntity();

            auto& transform = m_TexEntity.AddComponent<Components::Transform>();
            transform.Position = Math::Vector3f{ -0.5f, 0.f, 0.f };

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

            auto texMaterial = Renderer::Renderer::CreateMaterial(pipeline, "TexMat");

            texMaterial.Albedo = tex;
            texMaterial.Sampler = samp;

            auto& mesh = m_TexEntity.AddComponent<Components::MeshRenderer>();
            mesh.Material = texMaterial;
            mesh.Mesh.VertexBuffer = vertexBuffer;
            mesh.Mesh.VertexCount = 3;
        }
    }

    void SandboxLayer::OnEvent(Engine::Event& e) {
        Engine::EventDispatcher dispatcher(e);

        dispatcher.Dispatch<Engine::KeyPressedEvent>(
            [this](Engine::KeyPressedEvent& ke){
                auto& transform = m_CameraEntity.GetComponent<Components::Transform>();

                if (ke.GetKeyCode() == Engine::KeyCode::W) {
                    transform.Position.z += 1.f * m_DeltaTime;
                }
                if (ke.GetKeyCode() == Engine::KeyCode::A) {
                    transform.Position.x -= 1.f * m_DeltaTime;
                }
                if (ke.GetKeyCode() == Engine::KeyCode::S) {
                    transform.Position.z -= 1.f * m_DeltaTime;
                }
                if (ke.GetKeyCode() == Engine::KeyCode::D) {
                    transform.Position.x += 1.f * m_DeltaTime;
                }

                AETHER_INFO("Camera move: %f, %f, %f", transform.Position.x, transform.Position.y, transform.Position.z);

                return false;
            }
        );
    }

    void SandboxLayer::OnUpdate(float deltaTime) {
        m_DeltaTime = deltaTime;

        if (m_ActiveScene) {
            m_ActiveScene->OnUpdate(deltaTime);
        }
    }
}