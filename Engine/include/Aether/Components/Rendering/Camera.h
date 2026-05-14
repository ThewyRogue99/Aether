//
// Created by ThewyRogue99 on 6.04.2026.
//

#pragma once

#include <Aether/Core/Serialization/Archive.h>

namespace Aether::Components {
    struct Camera {
        float FOV = 45.0f;
        float Near = 0.1f;
        float Far = 100.0f;
        bool Primary = true;

        void Serialize(Serialization::Archive& ar) {
            ar("FOV", FOV);
            ar("Near", Near);
            ar("Far", Far);
            ar("Primary", Primary);
        }
    };
}
