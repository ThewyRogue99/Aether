//
// Created by ThewyRogue99 on 12.04.2026.
//

#pragma once

#include <vector>

#include <Aether/Core/Layer.h>
#include <Aether/Core/Memory/Pointer.h>

#include "EditorContext.h"
#include "Panels/EditorPanel.h"

namespace Aether {
    class EditorLayer : public Engine::Layer {
    public:
        EditorLayer();
        ~EditorLayer() override;

        void OnAttach() override;
        void OnDetach() override;

        void OnUpdate(float deltaTime) override;

        void OnEvent(Engine::Event& event) override;

    private:
        void BeginDockspace();
        void EndDockspace();

        template<typename T, typename... Args>
        T& AddPanel(Args&&... args) {
            auto panel = Engine::MakeScope<T>(std::forward<Args>(args)...);
            panel->OnAttach(m_Context);
            T& ref = *panel;
            m_Panels.push_back(std::move(panel));
            return ref;
        }

        EditorContext m_Context;
        std::vector<Engine::Scope<EditorPanel>> m_Panels;
    };
} // Aether
