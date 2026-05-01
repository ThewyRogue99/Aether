//
// Created by ThewyRogue99 on 8.01.2026.
//

#pragma once

#include <Aether/Events/Event.h>
#include <Aether/Input/MouseButton.h>

namespace Aether::Engine {
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) { }

        float GetX() const { return m_MouseX; }
        float GetY() const { return m_MouseY; }

        AETHER_EVENT_CLASS_TYPE(MouseMoved)
        AETHER_EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Mouse)

    private:
        float m_MouseX;
        float m_MouseY;
    };

    class MouseButtonEvent : public Event {
    public:
        MouseButton GetMouseButton() const { return m_Button; }

        AETHER_EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Mouse)

    protected:
        explicit MouseButtonEvent(MouseButton button) : m_Button(button) { }

        MouseButton m_Button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonPressedEvent(MouseButton button) : MouseButtonEvent(button) { }

        AETHER_EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonReleasedEvent(MouseButton button) : MouseButtonEvent(button) { }

        AETHER_EVENT_CLASS_TYPE(MouseButtonReleased)
    };
}
