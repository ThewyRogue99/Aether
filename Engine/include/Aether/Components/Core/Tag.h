//
// Created by ThewyRogue99 on 11.04.2026.
//

#pragma once

#include <vector>
#include <algorithm>

#include <Aether/Core/StringView.h>
#include <Aether/Core/Serialization/Archive.h>

namespace Aether::Components {
    struct Tag {
        std::vector<Engine::String> Tags;

        [[nodiscard]] bool HasTag(const Engine::String& tag) const {
            return std::any_of(
                Tags.begin(),
                Tags.end(),
                [&tag](const Engine::String& t) { return t == tag; }
            );
        }

        void AddTag(const Engine::String& tag) {
            if (!HasTag(tag)) Tags.emplace_back(tag);
        }

        void RemoveTag(const Engine::String& tag) {
            std::erase_if(Tags,[&tag](const Engine::String& t) { return t == tag; });
        }

        void Serialize(Serialization::Archive& ar) {
            if (ar.IsWriting()) {
                ar.BeginArray("Tags");
                for (auto& tag : Tags) {
                    ar.BeginArrayElement();
                    ar("value", tag);
                    ar.EndArrayElement();
                }
                ar.EndArray();
            } else {
                Tags.clear();
                ar.BeginArray("Tags");
                const size_t count = ar.ArraySize();
                for (size_t i = 0; i < count; ++i) {
                    ar.BeginArrayElement();
                    Engine::String tmp;
                    ar("value", tmp);
                    Tags.emplace_back(tmp);
                    ar.EndArrayElement();
                }
                ar.EndArray();
            }
        }
    };
}
