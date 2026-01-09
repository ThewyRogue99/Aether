//
// Created by ThewyRogue99 on 7.01.2026.
//

#pragma once

#include <Aether/Core/String.h>

namespace Aether::Engine {
    class Event;

    class Layer {
    public:
        explicit Layer(const String& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach();
        virtual void OnDetach();
        virtual void OnUpdate(float deltaTime);
        virtual void OnEvent(Event& event);

        const String& GetName() const;

    protected:
        String m_Name;
    };
}
