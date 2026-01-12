//
// Created by ThewyRogue99 on 11.01.2026.
//

#pragma once

#include <Aether/Renderer/RenderBackend.h>

namespace Aether::Platform {
    class GraphicsContext;
}

namespace Aether::Renderer {
    class GLContext;

    class GLBackend : public RenderBackend {
    public:
        GLBackend();
        ~GLBackend() override;

        [[nodiscard]] RenderAPI API() const override;

        void Init(const BackendInitInfo& info) override;
        void Shutdown() override;

        void BeginFrame() override;
        void EndFrame() override;

        void SetClearColor(float r, float g, float b, float a) override;
        void Clear() override;

        void Present() override;

    private:
        Platform::GraphicsContext* m_Context = nullptr;

        unsigned int m_Width  = 0;
        unsigned int m_Height = 0;

        float m_ClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
        bool m_InFrame = false;
    };
}
