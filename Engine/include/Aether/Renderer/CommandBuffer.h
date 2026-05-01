//
// Created by ThewyRogue99 on 11.04.2026.
//

#pragma once

#include <cstdint>
#include <vector>

#include <Aether/Math/Matrix.h>
#include <Aether/Renderer/Mesh.h>
#include <Aether/Renderer/Material.h>
#include <Aether/Renderer/RenderSurface.h>

namespace Aether::Renderer {
    enum class RenderCommandType : uint8_t {
        BeginFrame,
        EndFrame,
        SetClearColor,
        Clear,
        SetCamera,
        DrawMesh
    };

    struct BeginFrameCmd {
        RenderSurfaceHandle surface;
    };

    struct SetClearColorCmd {
        float r, g, b, a;
    };

    struct SetCameraCmd {
        Math::Matrix4f viewProjection;
    };

    struct DrawMeshCmd {
        Mesh mesh;
        Material material;
        Math::Matrix4f model;
    };

    struct RenderCommand {
        RenderCommandType type;

        union {
            BeginFrameCmd beginFrame;
            SetClearColorCmd setClearColor;
            SetCameraCmd setCamera;
            DrawMeshCmd drawMesh;
        };

        RenderCommand() : type(RenderCommandType::BeginFrame), beginFrame{} { }
    };

    class CommandBuffer {
    public:
        void Begin();
        void End();

        void BeginFrame(const RenderSurfaceHandle& surface = {});
        void EndFrame();

        void SetClearColor(float r, float g, float b, float a);
        void Clear();

        void SetCamera(const Math::Matrix4f& viewProjection);

        void DrawMesh(const Mesh& mesh, const Material& material, const Math::Matrix4f& model);

        [[nodiscard]] const std::vector<RenderCommand>& GetCommands() const;

        [[nodiscard]] bool IsRecording() const;

    private:
        std::vector<RenderCommand> m_Commands;
        bool m_Recording = false;
    };
}
