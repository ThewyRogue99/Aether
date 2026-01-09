//
// Created by ThewyRogue99 on 7.01.2026.
//

#include <Aether/Core/Layer.h>

namespace Aether::Engine {
    Layer::Layer(const String& name) : m_Name(name) { }

    Layer::~Layer() = default;

    void Layer::OnAttach() { }

    void Layer::OnDetach() { }

    void Layer::OnUpdate(float deltaTime) { }

    void Layer::OnEvent(Event& event) { }

    const String& Layer::GetName() const {
        return m_Name;
    }
}
