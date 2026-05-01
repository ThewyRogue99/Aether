//
// Created by ThewyRogue99 on 6.01.2026.
//

#pragma once

#include "Event.h"

namespace Aether::Engine {
    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() = default;

        AETHER_EVENT_CLASS_TYPE(WindowClose)
        AETHER_EVENT_CLASS_CATEGORY(EventCategory::Window | EventCategory::Application)
    };

    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(int width, int height)
            : m_Width(width), m_Height(height) { }

        [[nodiscard]] int GetWidth() const { return m_Width; }
        [[nodiscard]] int GetHeight() const { return m_Height; }

        AETHER_EVENT_CLASS_TYPE(WindowResize)
        AETHER_EVENT_CLASS_CATEGORY(EventCategory::Window)

    private:
        int m_Width;
        int m_Height;
    };

    class FramebufferResizeEvent : public Event {
    public:
        FramebufferResizeEvent(int width, int height)
            : m_Width(width), m_Height(height) { }

        [[nodiscard]] int GetWidth() const { return m_Width; }
        [[nodiscard]] int GetHeight() const { return m_Height; }

        AETHER_EVENT_CLASS_TYPE(FramebufferResize)
        AETHER_EVENT_CLASS_CATEGORY(EventCategory::Window | EventCategory::Application)

    private:
        int m_Width;
        int m_Height;
    };
}
