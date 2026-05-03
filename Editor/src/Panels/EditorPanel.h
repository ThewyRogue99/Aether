//
// Created by ThewyRogue99 on 12.04.2026.
//

#pragma once

namespace Aether {
    struct EditorContext;

    class EditorPanel {
    public:
        virtual ~EditorPanel() = default;

        virtual void OnAttach(EditorContext& context) { }
        virtual void OnDetach(EditorContext& context) { }

        virtual void Draw(EditorContext& context) = 0;

        [[nodiscard]] bool IsOpen() const { return m_Open; }
        void SetOpen(bool open) { m_Open = open; }

    protected:
        bool m_Open = true;
    };
} // Aether
