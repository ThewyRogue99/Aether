//
// Created by ThewyRogue99 on 15.04.2026.
//

#pragma once

#include <functional>
#include <vector>

#include <Aether/Core/String.h>

namespace Aether::Themes {
    using ApplyFn = std::function<void()>;

    struct ThemeEntry {
        Engine::String Name;
        ApplyFn Apply;
    };

    class ThemeRegistry {
    public:
        static ThemeRegistry& Get();

        // Register a theme. Later registrations with the same name overwrite earlier ones.
        void Register(Engine::String name, ApplyFn apply);

        // Apply a theme by name. Returns true if found.
        // Sets Current() on success.
        bool Apply(Engine::StringView name);

        [[nodiscard]] const std::vector<ThemeEntry>& All() const { return m_Themes; }
        [[nodiscard]] const Engine::String& Current() const { return m_Current; }

        ThemeRegistry(const ThemeRegistry&) = delete;
        ThemeRegistry& operator=(const ThemeRegistry&) = delete;

    private:
        ThemeRegistry()  = default;
        ~ThemeRegistry() = default;

        std::vector<ThemeEntry> m_Themes;
        Engine::String m_Current;
    };

    // Registers all built-in editor themes. Call once at startup.
    void RegisterBuiltins();
}
