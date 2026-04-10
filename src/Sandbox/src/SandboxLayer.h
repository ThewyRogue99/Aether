//
// Created by ThewyRogue99 on 8.01.2026.
//

#pragma once

#include "Aether/Core/Layer.h"

#include "Aether/Scene/Scene.h"
#include "Aether/Scene/Entity.h"

namespace Aether {
    class SandboxLayer : public Engine::Layer {
    public:
        SandboxLayer();
        ~SandboxLayer() override;

        void OnAttach() override;

        void OnEvent(Engine::Event& e) override;

        void OnUpdate(float deltaTime) override;

    private:
        Scene::Entity m_GreenEntity;
        Scene::Entity m_TexEntity;
        Scene::Entity m_CameraEntity;

        Engine::Scope<Scene::Scene> m_ActiveScene;

        float m_DeltaTime = 0.0f;
    };
}
