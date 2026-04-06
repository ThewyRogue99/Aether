//
// Created by ThewyRogue99 on 8.01.2026.
//

#pragma once

#include "Aether/Components/Core/Transform.h"
#include "Aether/Components/Physics/Velocity.h"
#include "Aether/Core/Layer.h"
#include "Aether/Events/EventDispatcher.h"
#include "Aether/Input/Input.h"
#include "Aether/Input/KeyCode.h"
#include "Aether/Log/Log.h"
#include "Aether/Events/KeyEvent.h"
#include "Aether/Math/Vector.h"
#include "Aether/Scene/Scene.h"
#include "Aether/Scene/Entity.h"
#include "Aether/Systems/Movement/MovementSystem.h"

using namespace Aether;

class SandboxLayer : public Engine::Layer {
public:
    SandboxLayer() : Layer("Gameplay") { }

    void OnAttach() override {
        m_ActiveScene = Engine::MakeScope<Scene::Scene>();

        m_ActiveScene->AddSystem<Systems::MovementSystem>();

        m_Entity = m_ActiveScene->CreateEntity();

        m_Entity.AddComponent<Components::Transform>();
        m_Entity.AddComponent<Components::Velocity>();
    }

    void OnEvent(Engine::Event& e) override {
        Engine::EventDispatcher dispatcher(e);

        dispatcher.Dispatch<Engine::KeyPressedEvent>(
            [this](Engine::KeyPressedEvent& ke){
                auto& velocity = m_Entity.GetComponent<Components::Velocity>();

                if (ke.GetKeyCode() == Engine::KeyCode::W) {
                    velocity.Value.y += 1.f;
                }
                if (ke.GetKeyCode() == Engine::KeyCode::A) {
                    velocity.Value.x -= 1.f;
                }
                if (ke.GetKeyCode() == Engine::KeyCode::S) {
                    velocity.Value.y -= 1.f;
                }
                if (ke.GetKeyCode() == Engine::KeyCode::D) {
                    velocity.Value.x += 1.f;
                }
                return false;
            }
        );
    }

    void OnUpdate(float deltaTime) override {
        if (m_ActiveScene) {
            m_ActiveScene->OnUpdate(deltaTime);

            for (const auto& [entt, transform] : m_ActiveScene->View<Components::Transform>().each()) {
                AETHER_INFO("%f, %f, %f", transform.Position.x, transform.Position.y, transform.Position.z);
            }
        }
    }

private:
    Scene::Entity m_Entity;

    Engine::Scope<Scene::Scene> m_ActiveScene;
};