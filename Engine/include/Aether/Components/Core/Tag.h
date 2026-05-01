//
// Created by ThewyRogue99 on 11.04.2026.
//

#pragma once

#include <vector>
#include <algorithm>

#include <Aether/Core/String.h>
#include <Aether/Core/StringView.h>

namespace Aether::Components {
    struct Tag {
        std::vector<Engine::String> Tags;

        [[nodiscard]] bool HasTag(const Engine::String& tag) const {
            return std::any_of(Tags.begin(), Tags.end(),
                [&tag](const Engine::String& t) { return t == tag; });
        }

        void AddTag(const Engine::String& tag) {
            if (!HasTag(tag))
                Tags.emplace_back(tag);
        }

        void RemoveTag(const Engine::String& tag) {
            std::erase_if(Tags,
                [&tag](const Engine::String& t) { return t == tag; });
        }
    };
}
