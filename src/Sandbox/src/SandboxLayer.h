//
// Created by ThewyRogue99 on 8.01.2026.
//

#pragma once

#include "Aether/Core/Layer.h"
#include "Aether/Events/EventDispatcher.h"
#include "Aether/Input/Input.h"
#include "Aether/Input/KeyCode.h"
#include "Aether/Log/Log.h"
#include "Aether/Events/KeyEvent.h"

using namespace Aether;

class SandboxLayer : public Engine::Layer
{
public:
    SandboxLayer() : Layer("Gameplay") { }

    void OnEvent(Engine::Event& e) override {
        Engine::EventDispatcher dispatcher(e);

        dispatcher.Dispatch<Engine::KeyPressedEvent>(
            [](Engine::KeyPressedEvent& ke){
                if (ke.GetKeyCode() == Engine::KeyCode::W) {
                    AETHER_INFO("ASD");
                }
                return false;
            }
        );
    }
};