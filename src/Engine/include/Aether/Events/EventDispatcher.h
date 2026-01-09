//
// Created by ThewyRogue99 on 6.01.2026.
//

#pragma once

#include <Aether/Core/Core.h>
#include <Aether/Events/Event.h>

namespace Aether::Engine {
    class AETHER_API EventDispatcher {
    public:
        explicit EventDispatcher(Event& event) : m_Event(event) {}

        template<typename T, typename F>
        bool Dispatch(const F& func) {
            if (m_Event.GetEventType() == T::GetStaticType()) {
                m_Event.Handled |= func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };
}
