//
// Created by ThewyRogue99 on 8.01.2026.
//

#pragma once

#include "Aether/Core/Layer.h"

#include "Aether/Scene/Entity.h"
#include <Aether/Renderer/RenderSurface.h>

namespace Aether {
    class SandboxLayer : public Engine::Layer {
    public:
        SandboxLayer();
        ~SandboxLayer() override;

        void OnAttach() override;

        void OnEvent(Engine::Event& e) override;

        void OnUpdate(float deltaTime) override;

    private:
        Renderer::RenderSurfaceHandle m_RenderSurfaceHandle;

        Scene::Entity m_GreenEntity;
        Scene::Entity m_TexEntity;
        Scene::Entity m_CameraEntity;

        float m_DeltaTime = 0.0f;
        float m_MoveSpeed = 5.0f;
        float m_MouseSensitivity = 0.1f;
        float m_LastMouseX = 0.0f;
        float m_LastMouseY = 0.0f;
    };
}
