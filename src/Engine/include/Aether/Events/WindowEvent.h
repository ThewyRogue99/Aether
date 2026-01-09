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

    class WindowResizeEvent final : public Event {
    public:
        WindowResizeEvent(uint32_t width, uint32_t height)
            : m_Width(width), m_Height(height) { }

        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }

        AETHER_EVENT_CLASS_TYPE(WindowResize)
        AETHER_EVENT_CLASS_CATEGORY(EventCategory::Window)

    private:
        uint32_t m_Width;
        uint32_t m_Height;
    };
}
