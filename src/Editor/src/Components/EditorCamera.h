//
// Created by ThewyRogue99 on 15.04.2026.
//

#pragma once

namespace Aether::Components {
    struct EditorCamera {
        float FOV = 60.f;
        float Near = 0.1f;
        float Far = 1000.f;

        float MoveSpeed = 5.f;
        float MouseSensitivity = 0.1f;
    };
}
