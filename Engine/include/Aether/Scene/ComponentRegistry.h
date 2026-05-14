//
// Created by ThewyRogue99 on 05.05.2026.
//

#pragma once

#include <functional>
#include <unordered_map>

#include <Aether/Core/Core.h>
#include <Aether/Core/Serialization/Archive.h>
#include <Aether/ECS/Entity.h>

#include "Scene.h"

namespace Aether::Scene {
    class AETHER_API ComponentRegistry {
    public:
        struct ComponentOps {
            std::function<void(Scene&, ECS::Entity)> Create;
            std::function<bool(const Scene&, ECS::Entity)> Has;
            std::function<void(Scene&, ECS::Entity, Serialization::Archive&)> Serialize;
        };

        static ComponentRegistry& Get();

        template<typename T>
        void Register(const char* name) {
            m_Components[name] = ComponentOps{
                .Create = [](Scene& scene, ECS::Entity e) {
                    scene.GetRegistry().AddComponent<T>(e);
                },
                .Has = [](const Scene& scene, ECS::Entity e) -> bool {
                    return scene.GetRegistry().HasComponent<T>(e);
                },
                .Serialize = [](Scene& scene, ECS::Entity e, Serialization::Archive& ar) {
                    scene.GetRegistry().GetComponent<T>(e).Serialize(ar);
                }
            };
        }

        void ForEach(const std::function<void(const Engine::String&, const ComponentOps&)>& fn) const;

        static void RegisterBuiltins();

    private:
        bool m_Initialized = false;
        std::unordered_map<Engine::String, ComponentOps> m_Components;
    };

} // namespace Aether::Scene
