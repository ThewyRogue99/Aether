//
// Created by ThewyRogue99 on 05.05.2026.
//

#pragma once

#include <Aether/Core/Core.h>
#include <Aether/Core/Serialization/Archive.h>

namespace Aether::Scene {
    class Scene;
}

namespace Aether::Scene {
    // Serialises an entire scene to/from an Archive.
    class AETHER_API SceneSerializer {
    public:
        static void Serialize(Scene& scene, Serialization::Archive& ar);

        static bool SaveToFile(Scene& scene, Engine::StringView path);

        static bool LoadFromFile(Scene& scene, Engine::StringView path);
    };
} // namespace Aether::Scene
