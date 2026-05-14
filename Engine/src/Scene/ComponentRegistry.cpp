//
// Created by ThewyRogue99 on 05.05.2026.
//

#include <Aether/Scene/ComponentRegistry.h>
#include <Aether/Scene/Scene.h>

// Built-in serialisable components
#include <Aether/Components/Core/Name.h>
#include <Aether/Components/Core/Tag.h>
#include <Aether/Components/Core/Transform.h>
#include <Aether/Components/Physics/Velocity.h>
#include <Aether/Components/Rendering/Camera.h>

namespace Aether::Scene {
    ComponentRegistry& ComponentRegistry::Get() {
        static ComponentRegistry instance;
        if (!instance.m_Initialized) {
            instance.m_Initialized = true;
            RegisterBuiltins();
        }
        return instance;
    }

    void ComponentRegistry::ForEach(const std::function<void(const Engine::String&, const ComponentOps&)>& fn) const {
        for (const auto& [name, ops] : m_Components) fn(name, ops);
    }

    void ComponentRegistry::RegisterBuiltins() {
        auto& reg = Get();
        reg.Register<Components::Name>("Name");
        reg.Register<Components::Tag>("Tag");
        reg.Register<Components::Transform>("Transform");
        reg.Register<Components::Velocity>("Velocity");
        reg.Register<Components::Camera>("Camera");
    }
} // namespace Aether::Scene
