//
// Created by ThewyRogue99 on 11.04.2026.
//

#pragma once

#include <Aether/Core/String.h>
#include <Aether/Core/Serialization/Archive.h>

namespace Aether::Components {
    struct Name {
        Engine::String Value;

        void Serialize(Serialization::Archive& ar) {
            ar("Value", Value);
        }
    };
}
