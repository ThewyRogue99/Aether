//
// Created by ThewyRogue99 on 10.04.2026.
//

#pragma once

#include <cstdint>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <functional>

#include <Aether/Core/Core.h>
#include <Aether/Core/Memory/Pointer.h>
#include <Aether/Scene/Scene.h>

namespace Aether::Scene {
    class AETHER_API SceneManager {
    public:
        SceneManager()  = default;
        ~SceneManager() = default;

        SceneManager(const SceneManager&)            = delete;
        SceneManager& operator=(const SceneManager&) = delete;

        template<typename T, typename... Args>
        void Register(std::string_view name, Args&&... args) {
            static_assert(std::is_base_of_v<Scene, T>,
                          "SceneManager::Register — T must inherit from Scene");

            auto factory = [args...]() -> Engine::Scope<Scene> {
                return Engine::MakeScope<T>(args...);
            };

            m_Registry[std::string(name)] = std::move(factory);
        }

        void LoadScene(std::string_view name);

        void PushScene(std::string_view name);

        void PopScene();

        void OnUpdate(float deltaTime);

        [[nodiscard]] Scene* GetActiveScene();
        [[nodiscard]] const Scene* GetActiveScene() const;

        [[nodiscard]] bool HasActiveScene() const;

        [[nodiscard]] std::size_t StackDepth() const;

    private:
        Scene* FindOrCreate(std::string_view name);

        using Factory = std::function<Engine::Scope<Scene>()>;

        std::unordered_map<std::string, Factory> m_Registry;

        std::unordered_map<std::string, Engine::Scope<Scene>> m_Loaded;

        std::vector<std::pair<std::string, Scene*>> m_Stack;
    };

} // namespace Aether::Scene