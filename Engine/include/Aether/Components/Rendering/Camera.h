//
// Created by ThewyRogue99 on 6.04.2026.
//

#pragma once

namespace Aether::Components {
    struct Camera {
        float FOV = 45.0f;
        float Near = 0.1f;
        float Far = 100.0f;

        bool Primary = true;
    };
}
