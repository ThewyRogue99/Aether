//
// Created by ThewyRogue99 on 8.01.2026.
//

#pragma once

#include <Aether/Events/Event.h>

#include <Aether/Input/KeyCode.h>

namespace Aether::Engine {
    class KeyEvent : public Event {
    public:
        KeyCode GetKeyCode() const { return m_KeyCode; }

        AETHER_EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Keyboard)

    protected:
        explicit KeyEvent(KeyCode keycode) : m_KeyCode(keycode) { }

        KeyCode m_KeyCode;
    };

    class KeyPressedEvent final : public KeyEvent {
    public:
        KeyPressedEvent(KeyCode keycode, bool repeat) : KeyEvent(keycode), m_IsRepeat(repeat) { }

        bool IsRepeat() const { return m_IsRepeat; }

        AETHER_EVENT_CLASS_TYPE(KeyPressed)

    private:
        bool m_IsRepeat;
    };

    class KeyReleasedEvent final : public KeyEvent {
    public:
        explicit KeyReleasedEvent(KeyCode keycode) : KeyEvent(keycode) { }

        AETHER_EVENT_CLASS_TYPE(KeyReleased)
    };
}
