//
// Created by ThewyRogue99 on 8.01.2026.
//

#include <Aether/Events/EventQueue.h>

#include <Aether/Events/Event.h>

namespace Aether::Engine {
    void EventQueue::Push(Scope<Event> event) {
        m_Queue.push(std::move(event));
    }

    Scope<Event> EventQueue::Pop() {
        auto e = std::move(m_Queue.front());
        m_Queue.pop();
        return e;
    }

    [[nodiscard]] bool EventQueue::Empty() const { return m_Queue.empty(); }
}
