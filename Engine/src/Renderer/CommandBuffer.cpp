//
// Created by ThewyRogue99 on 11.04.2026.
//

#include <Aether/Renderer/CommandBuffer.h>

namespace Aether::Renderer {
    void CommandBuffer::Begin() {
        m_Commands.clear();
        m_Recording = true;
    }

    void CommandBuffer::End() {
        m_Recording = false;
    }

    void CommandBuffer::BeginFrame(const RenderSurfaceHandle& surface) {
        RenderCommand cmd{};
        cmd.type = RenderCommandType::BeginFrame;
        cmd.beginFrame = { surface };
        m_Commands.push_back(cmd);
    }

    void CommandBuffer::EndFrame() {
        RenderCommand cmd{};
        cmd.type = RenderCommandType::EndFrame;
        m_Commands.push_back(cmd);
    }

    void CommandBuffer::SetClearColor(float r, float g, float b, float a) {
        RenderCommand cmd{};
        cmd.type = RenderCommandType::SetClearColor;
        cmd.setClearColor = { r, g, b, a };
        m_Commands.push_back(cmd);
    }

    void CommandBuffer::Clear() {
        RenderCommand cmd{};
        cmd.type = RenderCommandType::Clear;
        m_Commands.push_back(cmd);
    }

    void CommandBuffer::SetCamera(const Math::Matrix4f& viewProjection) {
        RenderCommand cmd{};
        cmd.type = RenderCommandType::SetCamera;
        cmd.setCamera = { viewProjection };
        m_Commands.push_back(cmd);
    }

    void CommandBuffer::DrawMesh(const Mesh& mesh, const Material& material, const Math::Matrix4f& model) {
        RenderCommand cmd{};
        cmd.type = RenderCommandType::DrawMesh;
        cmd.drawMesh = { mesh, material, model };
        m_Commands.push_back(cmd);
    }

    const std::vector<RenderCommand>& CommandBuffer::GetCommands() const {
        return m_Commands;
    }

    bool CommandBuffer::IsRecording() const {
        return m_Recording;
    }
}
