//
// Created by ThewyRogue99 on 12.04.2026.
//

#pragma once

#include "EditorPanel.h"

namespace Aether {
    class ViewportPanel : public EditorPanel {
    public:
        void Draw(EditorContext& context) override;

        bool IsHovered() const { return m_IsHovered; }

    private:
        bool m_IsHovered = false;
    };
} // Aether
