//
// Created by ThewyRogue99 on 05.05.2026.
//

#include <Aether/Scene/SceneSerializer.h>

#include <Aether/Core/Serialization/JsonReadArchive.h>
#include <Aether/Core/Serialization/JsonWriteArchive.h>
#include <Aether/Log/Log.h>
#include <Aether/Scene/ComponentRegistry.h>
#include <Aether/Scene/Scene.h>
#include <Aether/Components/Core/Name.h>

namespace Aether::Scene {
    void SceneSerializer::Serialize(Scene& scene, Serialization::Archive& ar) {
        auto& registry = ComponentRegistry::Get();

        if (ar.IsWriting()) {
            // ── Write ──────────────────────────────────────────────────────────────
            ar.BeginArray("entities");

            // Every entity has a Name component — use it as the iteration anchor.
            for (auto [entity, _] : scene.View<Components::Name>().each()) {
                ar.BeginArrayElement();

                ar.BeginObject("components");
                registry.ForEach([&](const Engine::String& name, const ComponentRegistry::ComponentOps& ops) {
                    if (ops.Has(scene, entity)) {
                        ar.BeginObject(name);
                        ops.Serialize(scene, entity, ar);
                        ar.EndObject();
                    }
                });
                ar.EndObject(); // components

                ar.EndArrayElement();
            }

            ar.EndArray(); // entities

        } else {
            // ── Read ───────────────────────────────────────────────────────────────
            ar.BeginArray("entities");
            const size_t count = ar.ArraySize();

            for (size_t i = 0; i < count; ++i) {
                ar.BeginArrayElement();

                // CreateEntity() already adds Name and Tag with defaults.
                ECS::Entity entity = scene.CreateEntity().GetHandle();

                ar.BeginObject("components");
                registry.ForEach([&](const Engine::String& name, const ComponentRegistry::ComponentOps& ops) {
                    if (!ar.HasKey(name)) return;

                    // Ensure the component exists before deserialising into it.
                    if (!ops.Has(scene, entity))
                        ops.Create(scene, entity);

                    ar.BeginObject(name);
                    ops.Serialize(scene, entity, ar);
                    ar.EndObject();
                });
                ar.EndObject(); // components

                ar.EndArrayElement();
            }

            ar.EndArray(); // entities
        }
    }

    bool SceneSerializer::SaveToFile(Scene& scene, Engine::StringView path) {
        Serialization::JsonWriteArchive ar;
        Serialize(scene, ar);
        return ar.SaveToFile(path);
    }

    bool SceneSerializer::LoadFromFile(Scene& scene, Engine::StringView path) {
        auto ar = Serialization::JsonReadArchive::FromFile(path);
        if (!ar.IsValid()) {
            AETHER_ERROR("SceneSerializer: failed to load '%.*s'",
                         static_cast<int>(path.size()), path.data());
            return false;
        }
        Serialize(scene, ar);
        return true;
    }
} // namespace Aether::Scene
