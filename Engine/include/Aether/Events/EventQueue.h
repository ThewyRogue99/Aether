//
// Created by ThewyRogue99 on 8.01.2026.
//

#pragma once

#include <queue>

#include <Aether/Core/Memory/Pointer.h>

namespace Aether::Engine {
    class Event;

    class EventQueue {
    public:
        void Push(Scope<Event> event);
        Scope<Event> Pop();

        [[nodiscard]] bool Empty() const;

    private:
        std::queue<Scope<Event>> m_Queue;
    };
}
