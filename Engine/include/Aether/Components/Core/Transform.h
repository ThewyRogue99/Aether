//
// Created by ThewyRogue99 on 5.04.2026.
//

#pragma once

#include <Aether/Math/Vector.h>

namespace Aether::Components {
    struct Transform {
        Math::Vector3f Position{ 0.f, 0.f, 0.f };
        Math::Vector3f Rotation{ 0.f, 0.f, 0.f };
        Math::Vector3f Scale{ 1.f, 1.f, 1.f };
    };
}
