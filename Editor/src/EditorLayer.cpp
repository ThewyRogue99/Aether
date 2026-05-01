//
// Created by ThewyRogue99 on 12.04.2026.
//

#include "EditorLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include <Aether/Core/Assert.h>
#include <Aether/Core/Application.h>
#include <Aether/Core/RuntimeObject.h>
#include <Aether/Platform/Window.h>
#include <Aether/Renderer/Renderer.h>
#include <Aether/Scene/Scene.h>
#include <Aether/Scene/SceneManager.h>
#include <Aether/Systems/Rendering/RenderingSystem.h>
#include <Aether/Systems/Movement/MovementSystem.h>
#include <Aether/Components/Core/Name.h>
#include <Aether/Components/Core/Transform.h>
#include <Aether/Components/Rendering/Camera.h>
#include <Aether/Components/Rendering/MeshRenderer.h>

#include <Aether/Input/Input.h>
#include <Aether/Input/KeyCode.h>
#include <Aether/Input/MouseButton.h>
#include <Aether/Math/Math.h>

#include "EditorRenderingSystem.h"
#include "EditorFonts.h"
#include "Components/EditorCamera.h"
#include "Panels/ViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Themes/ThemeRegistry.h"

namespace Aether {
    // ── Temporary test scene ──────────────────────────────────────
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

    class EditorScene : public Engine::RuntimeObject<EditorScene, Scene::Scene> {
    public:
        void OnLoad() override {
            AddSystem<Systems::MovementSystem>(0);

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

            // Green triangle
            {
                auto e = CreateEntity("Green Triangle");
                auto& t = e.AddComponent<Components::Transform>();
                t.Position = Math::Vector3f{ 1.f, 0.f, 0.f };

                auto mat = Renderer::Renderer::CreateMaterial(pipeline, "GreenMat");
                Renderer::Renderer::SetMaterialColor(mat, { 0, 1, 0, 1 });

                auto& mesh = e.AddComponent<Components::MeshRenderer>();
                mesh.Material = mat;
                mesh.Mesh.VertexBuffer = vertexBuffer;
                mesh.Mesh.VertexCount = 3;
            }

            // White triangle
            {
                auto e = CreateEntity("White Triangle");
                auto& t = e.AddComponent<Components::Transform>();
                t.Position = Math::Vector3f{ -1.f, 0.f, 0.f };
                t.Scale = Math::Vector3f{ 1.5f, 1.5f, 1.5f };

                auto mat = Renderer::Renderer::CreateMaterial(pipeline, "WhiteMat");

                auto& mesh = e.AddComponent<Components::MeshRenderer>();
                mesh.Material = mat;
                mesh.Mesh.VertexBuffer = vertexBuffer;
                mesh.Mesh.VertexCount = 3;
            }
        }

        void OnUnload() override { }
    };

    // ── EditorLayer ───────────────────────────────────────────────

    EditorLayer::EditorLayer() = default;

    EditorLayer::~EditorLayer() = default;

    void EditorLayer::OnAttach() {
        Layer::OnAttach();

        auto api = Renderer::Renderer::GetAPI();
        AETHER_ASSERT_MSG(api == Renderer::RenderAPI::OpenGL, "Editor only supports OpenGL for now");

        // Create off-screen surface for the viewport
        m_Context.ViewportSurface = Renderer::Renderer::CreateRenderSurface({
            .width = 1280,
            .height = 720,
            .colorFormat = Renderer::TextureFormat::RGBA8,
            .debugName = "EditorViewport"
        });

        // ImGui init
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        Themes::RegisterBuiltins();
        ApplyTheme("Aether Dark");

        EditorFonts::Load();

        auto* window = static_cast<GLFWwindow*>(
            Engine::Application::Get().GetWindow().GetNativeHandle()
        );

        ImGui_ImplGlfw_InitForOpenGL(window, true);

        Renderer::Renderer::SubmitAndFlush([]() {
            ImGui_ImplOpenGL3_Init("#version 410");
        });

        // Panels
        m_ViewportPanel = &AddPanel<ViewportPanel>();
        AddPanel<SceneHierarchyPanel>();
        AddPanel<PropertiesPanel>();

        // Load test scene
        auto& sm = Engine::Application::Get().GetSceneManager();
        sm.Register<EditorScene>("EditorScene");
        sm.LoadScene("EditorScene");

        if (auto* scene = sm.GetActiveScene()) {
            m_RenderingSystem = &scene->AddSystem<EditorRenderingSystem>(
                1, m_Context.ViewportSurface);

            m_EditorCameraEntity = scene->CreateEntity("Editor Camera");
            auto& transform = m_EditorCameraEntity.AddComponent<Components::Transform>();
            transform.Position = { 0.f, 0.f, -5.f };
            m_EditorCameraEntity.AddComponent<Components::EditorCamera>();

            m_Context.ActiveScene = scene;
            m_Context.SelectedEntity = {};
        }
    }

    void EditorLayer::OnDetach() {
        for (auto& panel : m_Panels)
            panel->OnDetach(m_Context);

        Renderer::Renderer::SubmitAndFlush([]() {
            ImGui_ImplOpenGL3_Shutdown();
        });

        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        if (m_Context.ViewportSurface) {
            Renderer::Renderer::DestroyRenderSurface(m_Context.ViewportSurface);
        }

        Layer::OnDetach();
    }

    void EditorLayer::OnUpdate(float deltaTime) {
        Layer::OnUpdate(deltaTime);

        // ImGui frame (CPU side)
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        BeginDockspace();

        for (auto& panel : m_Panels) {
            if (panel->IsOpen()) {
                panel->Draw(m_Context);
            }
        }

        EndDockspace();

        UpdateEditorCamera(deltaTime);

        ImGui::Render();

        const bool viewportsEnabled =
            (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0;

        // Native window creation/destruction MUST happen on the main thread
        // (macOS NSWindow restriction). The actual GL rendering for those
        // viewports is done on the render thread below.
        if (viewportsEnabled) {
            ImGui::UpdatePlatformWindows();
        }

        // Render ImGui to the default framebuffer (FBO 0) on the render thread,
        // render any extra viewport windows there too, then swap buffers.
        auto* context = Engine::Application::Get().GetWindow().GetGraphicsContext();
        Renderer::Renderer::SubmitAndFlush([context, viewportsEnabled]() {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (viewportsEnabled) {
                // RenderPlatformWindowsDefault makes each viewport's GL context
                // current per window, so save/restore around it.
                GLFWwindow* backup = glfwGetCurrentContext();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup);
            }

            context->SwapBuffers();
        });
    }

    void EditorLayer::OnEvent(Engine::Event& event) {
        Layer::OnEvent(event);
    }

    void EditorLayer::UpdateEditorCamera(float deltaTime) {
        const float mouseX = Engine::Input::GetMouseX();
        const float mouseY = Engine::Input::GetMouseY();

        const bool active = m_EditorCameraEntity
            && m_ViewportPanel
            && m_ViewportPanel->IsHovered()
            && Engine::Input::IsMouseButtonPressed(Engine::MouseButton::Right);

        if (active) {
            auto& transform = m_EditorCameraEntity.GetComponent<Components::Transform>();
            auto& editorCamera = m_EditorCameraEntity.GetComponent<Components::EditorCamera>();

            const float dx = mouseX - m_LastMouseX;
            const float dy = mouseY - m_LastMouseY;

            transform.Rotation.y -= dx * editorCamera.MouseSensitivity;
            transform.Rotation.x -= dy * editorCamera.MouseSensitivity;

            if (transform.Rotation.x > 89.f) transform.Rotation.x =  89.f;
            if (transform.Rotation.x < -89.f) transform.Rotation.x = -89.f;

            const float pitch = Math::Radians(transform.Rotation.x);
            const float yaw = Math::Radians(transform.Rotation.y);

            Math::Vector3f forward = Math::Normalize(Math::Vector3f{
                std::cos(pitch) * std::sin(yaw),
                std::sin(pitch),
                std::cos(pitch) * std::cos(yaw)
            });
            const Math::Vector3f worldUp = { 0.f, 1.f, 0.f };
            Math::Vector3f right = Math::Normalize(Math::Cross(forward, worldUp));

            float speed = editorCamera.MoveSpeed;
            if (Engine::Input::IsKeyPressed(Engine::KeyCode::LeftShift)) speed *= 2.5f;

            const float step = speed * deltaTime;

            if (Engine::Input::IsKeyPressed(Engine::KeyCode::W))
                transform.Position += forward * step;
            if (Engine::Input::IsKeyPressed(Engine::KeyCode::S))
                transform.Position -= forward * step;
            if (Engine::Input::IsKeyPressed(Engine::KeyCode::D))
                transform.Position += right * step;
            if (Engine::Input::IsKeyPressed(Engine::KeyCode::A))
                transform.Position -= right * step;
            if (Engine::Input::IsKeyPressed(Engine::KeyCode::Space))
                transform.Position.y += step;
            if (Engine::Input::IsKeyPressed(Engine::KeyCode::C))
                transform.Position.y -= step;
        }

        m_LastMouseX = mouseX;
        m_LastMouseY = mouseY;
    }

    void EditorLayer::BeginDockspace() {
        ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_MenuBar |
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace", nullptr, windowFlags);
        ImGui::PopStyleVar(3);

        DrawMainMenuBar();

        ImGui::DockSpace(ImGui::GetID("EditorDockSpace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    }

    void EditorLayer::EndDockspace() {
        ImGui::End();
    }

    void EditorLayer::DrawMainMenuBar() {
        if (!ImGui::BeginMenuBar()) return;

        if (ImGui::BeginMenu("View")) {
            if (ImGui::BeginMenu("Theme")) {
                const auto& reg     = Themes::ThemeRegistry::Get();
                const auto& current = reg.Current();

                for (const auto& entry : reg.All()) {
                    const bool selected = (entry.Name == current);
                    if (ImGui::MenuItem(entry.Name.c_str(), nullptr, selected)) {
                        ApplyTheme(entry.Name);
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    void EditorLayer::ApplyTheme(std::string_view name) {
        if (!Themes::ThemeRegistry::Get().Apply(name)) return;

        // When multi-viewport is enabled, OS-level windows can't have transparent
        // or rounded corners — force opaque/square so they don't look broken.
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGuiStyle& style = ImGui::GetStyle();
            style.WindowRounding              = 0.f;
            style.Colors[ImGuiCol_WindowBg].w = 1.f;
        }
    }
} // Aether
