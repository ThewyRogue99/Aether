//
// Created by ThewyRogue99 on 6.01.2026.
//

#pragma once

#include <cstdint>

#include <Aether/Core/Core.h>

#define AETHER_EVENT_CLASS_TYPE(type) \
static EventType GetStaticType() { return EventType::type; } \
virtual EventType GetEventType() const override { return GetStaticType(); } \
virtual const char* GetName() const override { return #type; }

#define AETHER_EVENT_CLASS_CATEGORY(category) \
virtual EventCategory GetCategoryFlags() const override { return category; }

namespace Aether::Engine {
    enum class AETHER_API EventCategory : uint32_t {
        None        = 0,
        Application = 1 << 0,
        Window      = 1 << 1,
        Input       = 1 << 2,
        Keyboard    = 1 << 3,
        Mouse       = 1 << 4,
    };

    AETHER_API inline EventCategory operator|(EventCategory a, EventCategory b) {
        return static_cast<EventCategory>(
            static_cast<uint32_t>(a) | static_cast<uint32_t>(b)
        );
    }

    enum class AETHER_API EventType : uint8_t {
        None = 0,

        AppTick,
        AppUpdate,
        AppRender,

        WindowClose,
        WindowResize,
        FramebufferResize,

        KeyPressed,
        KeyReleased,
        MouseMoved,
        MouseButtonPressed,
        MouseButtonReleased,
    };

    class AETHER_API Event {
    public:
        virtual ~Event() = default;

        [[nodiscard]] virtual EventType GetEventType() const = 0;
        [[nodiscard]] virtual const char* GetName() const = 0;
        [[nodiscard]] virtual EventCategory GetCategoryFlags() const = 0;

        [[nodiscard]] bool IsInCategory(EventCategory category) const {
            return static_cast<uint32_t>(GetCategoryFlags()) &
                   static_cast<uint32_t>(category);
        }

    public:
        bool Handled = false;
    };
}
