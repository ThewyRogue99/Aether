//
// Created by ThewyRogue99 on 15.04.2026.
//

#include "ThemeRegistry.h"

#include "Aether.h"
#include "Catppuccin.h"
#include "Crimson.h"
#include "DarkBlue.h"
#include "Gruvbox.h"
#include "MidnightForge.h"
#include "Sapphire.h"

namespace Aether::Themes {
    ThemeRegistry& ThemeRegistry::Get() {
        static ThemeRegistry instance;
        return instance;
    }

    void ThemeRegistry::Register(Engine::String name, ApplyFn apply) {
        for (auto& entry : m_Themes) {
            if (entry.Name == name) {
                entry.Apply = std::move(apply);
                return;
            }
        }
        m_Themes.push_back({ std::move(name), std::move(apply) });
    }

    bool ThemeRegistry::Apply(Engine::StringView name) {
        for (const auto&[Name, Apply] : m_Themes) {
            if (Name == name) {
                Apply();
                m_Current.assign(name);
                return true;
            }
        }
        return false;
    }

    void RegisterBuiltins() {
        auto& reg = ThemeRegistry::Get();

        reg.Register("Aether Dark", &ApplyAetherDark);

        reg.Register("Midnight Forge", &ApplyMidnightForgeDark);
        reg.Register("Midnight Forge (Light)", &ApplyMidnightForgeLight);

        reg.Register("Catppuccin Latte", &ApplyCatppuccinLatte);
        reg.Register("Catppuccin Frappé", &ApplyCatppuccinFrappe);
        reg.Register("Catppuccin Macchiato", &ApplyCatppuccinMacchiato);
        reg.Register("Catppuccin Mocha", &ApplyCatppuccinMocha);

        reg.Register("Crimson Vesuvius", &ApplyCrimsonVesuvius);
        reg.Register("Sapphire", &ApplySapphire);
        reg.Register("Gruvbox Hard", &ApplyGruvboxHard);
        reg.Register("Dark Blue", &ApplyDarkBlue);
    }
}
