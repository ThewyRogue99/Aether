//
// Created by ThewyRogue99 on 8.01.2026.
//

#pragma once

#include <vector>

#include <Aether/Core/Memory/Pointer.h>

namespace Aether::Engine {
    class Layer;

    class LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Scope<Layer> layer);
        void PushOverlay(Scope<Layer> overlay);

        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        std::vector<Scope<Layer>>::iterator begin();
        std::vector<Scope<Layer>>::iterator end();

        std::vector<Scope<Layer>>::reverse_iterator rbegin();
        std::vector<Scope<Layer>>::reverse_iterator rend();

    private:
        std::vector<Scope<Layer>> m_Layers;

        uint32_t m_LayerInsertIndex = 0;
    };
}
