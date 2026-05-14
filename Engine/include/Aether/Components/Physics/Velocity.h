//
// Created by ThewyRogue99 on 5.04.2026.
//

#pragma once

#include <Aether/Math/Vector.h>
#include <Aether/Core/Serialization/Archive.h>

namespace Aether::Components {
    struct Velocity {
        Math::Vector3f Value{ 0.f, 0.f, 0.f };

        void Serialize(Serialization::Archive& ar) {
            ar("Value", Value);
        }
    };
}
