//
// Created by ThewyRogue99 on 6.04.2026.
//

#include "SandboxLayer.h"

#include <Aether/Components/Core/Transform.h>
#include <Aether/Components/Physics/Velocity.h>
#include <Aether/Components/Rendering/Camera.h>
#include <Aether/Components/Rendering/MeshRenderer.h>
#include <Aether/Core/Application.h>
#include <Aether/Events/EventDispatcher.h>
#include <Aether/Events/KeyEvent.h>
#include <Aether/Input/Input.h>
#include <Aether/Input/KeyCode.h>
#include <Aether/Log/Log.h>
#include <Aether/Math/Math.h>
#include <Aether/Math/Vector.h>
#include <Aether/Renderer/Renderer.h>
#include <Aether/Scene/Scene.h>
#include <Aether/Scene/SceneManager.h>
#include <Aether/Systems/Movement/MovementSystem.h>
#include <Aether/Systems/Rendering/RenderingSystem.h>
#include <Aether/Scene/Entity.h>

namespace Aether {
    struct Vertex {
        float position[3];
        float normal[3];
        float texCoord[2];
    };

    static Vertex s_Vertices[3] = {
        {{  0.0f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.5f, 1.0f }},
        {{  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }},
        {{ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }}
    };

    static const char* s_VertexSrc = R"(#version 330 core
        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec3 a_Normal;
        layout(location = 2) in vec2 a_TexCoord;

        layout(std140) uniform Camera { mat4 u_ViewProjection; };
        layout(std140) uniform Object { mat4 u_Model; };

        out vec2 v_TexCoord;

        void main() {
            v_TexCoord  = a_TexCoord;
            gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
        }
    )";

    static const char* s_FragmentSrc = R"(#version 330 core
        layout(std140) uniform Material { vec4 u_BaseColor; };

        in  vec2 v_TexCoord;
        out vec4 FragColor;

        uniform sampler2D u_Albedo;

        void main() {
            vec4 tex  = texture(u_Albedo, v_TexCoord);
            FragColor = tex * u_BaseColor;
        }
    )";

    class GameplayScene : public Engine::RuntimeObject<GameplayScene, Scene::Scene> {
    public:
        Aether::Scene::Entity OutGreenEntity;
        Aether::Scene::Entity OutTexEntity;
        Aether::Scene::Entity OutCameraEntity;

        void OnLoad() override {
            AddSystem<Systems::MovementSystem>(0);
            AddSystem<Systems::RenderingSystem>(1);

            // Shared GPU resources
            const auto vertexBuffer = Renderer::Renderer::CreateBuffer({
                .usage  = Renderer::BufferUsage::Vertex,
                .access = Renderer::BufferAccess::Static,
                .size   = sizeof(s_Vertices)
            }, s_Vertices);

            const auto shader = Renderer::Renderer::CreateShader({
                .debugName      = "TriangleShader",
                .vertexSource   = s_VertexSrc,
                .fragmentSource = s_FragmentSrc
            });

            Renderer::VertexAttribute attributes[] = {
                { .location = 0, .format = Renderer::VertexFormat::Float3,
                  .offset = offsetof(Vertex, position), .normalized = false },
                { .location = 1, .format = Renderer::VertexFormat::Float3,
                  .offset = offsetof(Vertex, normal),   .normalized = false },
                { .location = 2, .format = Renderer::VertexFormat::Float2,
                  .offset = offsetof(Vertex, texCoord), .normalized = false }
            };

            Renderer::PipelineLayoutDesc layoutDesc;
            layoutDesc.vertexLayout = {
                .attributes     = attributes,
                .attributeCount = 3,
                .stride         = sizeof(Vertex)
            };
            layoutDesc.uniformBufferLayout = {
                { "Camera",   0, Renderer::ShaderStage::Vertex,   Renderer::UniformScope::Global   },
                { "Object",   1, Renderer::ShaderStage::Vertex,   Renderer::UniformScope::Object   },
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

            // Camera entity
            {
                auto e = CreateEntity();
                auto& t   = e.AddComponent<Components::Transform>();
                t.Position = Math::Vector3f{ 0.f, 0.f, -4.f };

                auto& cam  = e.AddComponent<Components::Camera>();
                cam.FOV    = 60.f;
                cam.Near   = 0.1f;
                cam.Far    = 100.f;
                cam.Primary = true;

                if (!OutCameraEntity) OutCameraEntity = e;
            }

            // Green triangle
            {
                auto e = CreateEntity();
                auto& t   = e.AddComponent<Components::Transform>();
                t.Position = Math::Vector3f{ 1.f, 0.f, 0.f };

                auto mat = Renderer::Renderer::CreateMaterial(pipeline, "GreenMat");
                Renderer::Renderer::SetMaterialColor(mat, { 0, 1, 0, 1 });

                auto& mesh         = e.AddComponent<Components::MeshRenderer>();
                mesh.Material      = mat;
                mesh.Mesh.VertexBuffer = vertexBuffer;
                mesh.Mesh.VertexCount  = 3;

                if (!OutGreenEntity) OutGreenEntity = e;
            }

            // Textured triangle
            {
                auto e = CreateEntity();
                auto& t   = e.AddComponent<Components::Transform>();
                t.Position = Math::Vector3f{ -1.f, 0.f, 0.f };
                t.Scale    = Math::Vector3f{  1.5f, 1.5f, 1.5f };

                uint8_t pixels[] = {
                    255, 255, 255, 255,
                    0,   0,   0,   255,
                    0,   0,   0,   255,
                    255, 255, 255, 255
                };

                Renderer::TextureDesc td{};
                td.width  = 2;
                td.height = 2;
                td.format = Renderer::TextureFormat::RGBA8;
                td.data   = pixels;

                const auto tex  = Renderer::Renderer::CreateTexture2D(td);

                Renderer::SamplerDesc sd{};
                sd.min = Renderer::Filter::Nearest;
                sd.mag = Renderer::Filter::Nearest;

                const auto samp = Renderer::Renderer::CreateSampler(sd);

                auto mat      = Renderer::Renderer::CreateMaterial(pipeline, "TexMat");
                mat.Albedo    = tex;
                mat.Sampler   = samp;

                auto& mesh         = e.AddComponent<Components::MeshRenderer>();
                mesh.Material      = mat;
                mesh.Mesh.VertexBuffer = vertexBuffer;
                mesh.Mesh.VertexCount  = 3;

                if (!OutTexEntity) OutTexEntity = e;
            }

            AETHER_INFO("GameplayScene: loaded");
        }

        void OnUnload() override {
            AETHER_INFO("GameplayScene: unloaded");
        }
    };

    // -----------------------------------------------------------------------
    // SandboxLayer
    // -----------------------------------------------------------------------

    SandboxLayer::SandboxLayer() : Layer("Gameplay") { }

    SandboxLayer::~SandboxLayer() = default;

    void SandboxLayer::OnAttach() {
        auto& sm = Engine::Application::Get().GetSceneManager();
        Engine::Application::Get().GetSceneManager().Register<GameplayScene>("Gameplay");

        sm.LoadScene("Gameplay");

        // After loading, grab our entity handles from the scene.
        if (auto* scene = static_cast<GameplayScene*>(sm.GetActiveScene())) {
            m_GreenEntity = scene->OutGreenEntity;
            m_TexEntity = scene->OutTexEntity;
            m_CameraEntity = scene->OutCameraEntity;
        }
    }

    void SandboxLayer::OnEvent(Engine::Event& e) {
        Engine::EventDispatcher dispatcher(e);

        dispatcher.Dispatch<Engine::KeyPressedEvent>(
            [this](Engine::KeyPressedEvent& ke) {
                if (!m_CameraEntity) return false;

                auto& transform = m_CameraEntity.GetComponent<Components::Transform>();

                if (ke.GetKeyCode() == Engine::KeyCode::W) transform.Position.z += 1.f * m_DeltaTime;
                if (ke.GetKeyCode() == Engine::KeyCode::A) transform.Position.x -= 1.f * m_DeltaTime;
                if (ke.GetKeyCode() == Engine::KeyCode::S) transform.Position.z -= 1.f * m_DeltaTime;
                if (ke.GetKeyCode() == Engine::KeyCode::D) transform.Position.x += 1.f * m_DeltaTime;

                AETHER_INFO("Camera: %.2f, %.2f, %.2f",
                    transform.Position.x,
                    transform.Position.y,
                    transform.Position.z);

                return false;
            });
    }

    static constexpr float k_RotationSpeed = 40.f;

    void SandboxLayer::OnUpdate(float deltaTime) {
        m_DeltaTime = deltaTime;

        if (m_TexEntity) {
            auto& t = m_TexEntity.GetComponent<Components::Transform>();
            t.Rotation.z += k_RotationSpeed * deltaTime;
        }
    }

}
