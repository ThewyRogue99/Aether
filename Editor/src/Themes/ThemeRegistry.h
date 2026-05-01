//
// Created by ThewyRogue99 on 15.04.2026.
//

#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace Aether::Themes {
    using ApplyFn = std::function<void()>;

    struct ThemeEntry {
        std::string Name;
        ApplyFn     Apply;
    };

    class ThemeRegistry {
    public:
        static ThemeRegistry& Get();

        // Register a theme. Later registrations with the same name overwrite earlier ones.
        void Register(std::string name, ApplyFn apply);

        // Apply a theme by name. Returns true if found.
        // Sets Current() on success.
        bool Apply(std::string_view name);

        [[nodiscard]] const std::vector<ThemeEntry>& All() const { return m_Themes; }
        [[nodiscard]] const std::string& Current() const { return m_Current; }

    private:
        ThemeRegistry()  = default;
        ~ThemeRegistry() = default;
        ThemeRegistry(const ThemeRegistry&)            = delete;
        ThemeRegistry& operator=(const ThemeRegistry&) = delete;

        std::vector<ThemeEntry> m_Themes;
        std::string             m_Current;
    };

    // Registers all built-in editor themes. Call once at startup.
    void RegisterBuiltins();
}
