//
// Created by ThewyRogue99 on 8.01.2026.
//

#include <Aether/Core/LayerStack.h>

#include <Aether/Core/Layer.h>

namespace Aether::Engine {
    LayerStack::LayerStack() = default;

    LayerStack::~LayerStack() = default;

    void LayerStack::PushLayer(Scope<Layer> layer) {
        layer->OnAttach();

        m_Layers.emplace(
            m_Layers.begin() + m_LayerInsertIndex,
            std::move(layer)
        );
        m_LayerInsertIndex++;
    }

    void LayerStack::PushOverlay(Scope<Layer> overlay) {
        overlay->OnAttach();
        m_Layers.emplace_back(std::move(overlay));
    }

    void LayerStack::PopLayer(Layer *layer) {
        const auto it = std::find_if(
            m_Layers.begin(),
            m_Layers.begin() + m_LayerInsertIndex,
            [layer](const std::unique_ptr<Layer>& ptr)
            {
                return ptr.get() == layer;
            }
        );

        if (it != m_Layers.begin() + m_LayerInsertIndex) {
            (*it)->OnDetach();
            m_Layers.erase(it);
            m_LayerInsertIndex--;
        }
    }

    void LayerStack::PopOverlay(Layer *overlay) {
        const auto it = std::find_if(
            m_Layers.begin() + m_LayerInsertIndex,
            m_Layers.end(),
            [overlay](const std::unique_ptr<Layer>& ptr) {
                return ptr.get() == overlay;
            }
        );

        if (it != m_Layers.end()) {
            (*it)->OnDetach();
            m_Layers.erase(it);
        }
    }

    std::vector<Scope<Layer>>::iterator LayerStack::begin() {
        return m_Layers.begin();
    }

    std::vector<Scope<Layer>>::iterator LayerStack::end() {
        return m_Layers.end();
    }

    std::vector<Scope<Layer>>::reverse_iterator LayerStack::rbegin() {
        return m_Layers.rbegin();
    }

    std::vector<Scope<Layer>>::reverse_iterator LayerStack::rend() {
        return m_Layers.rend();
    }
}
