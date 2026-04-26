//
// Created by ThewyRogue99 on 12.04.2026.
//

#pragma once

#include "EditorPanel.h"

namespace Aether {
    namespace Scene {
        class Scene;
    }

    class SceneHierarchyPanel : public EditorPanel {
    public:
        SceneHierarchyPanel();
        ~SceneHierarchyPanel() override;

        void OnAttach(EditorContext &context) override;
        void OnDetach(EditorContext &context) override;

        void Draw(EditorContext& context) override;
    };
} // Aether
