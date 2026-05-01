//
// Created by ThewyRogue99 on 10.04.2026.
//

#include <Aether/Scene/SceneManager.h>
#include <Aether/Core/Assert.h>
#include <Aether/Log/Log.h>

namespace Aether::Scene {
    Scene* SceneManager::FindOrCreate(std::string_view name) {
        const std::string key(name);

        if (const auto it = m_Loaded.find(key); it != m_Loaded.end()) return it->second.get();

        const auto factoryIt = m_Registry.find(key);
        AETHER_ASSERT_MSG(
            factoryIt != m_Registry.end(),
            "SceneManager: scene \"%s\" has not been registered", key.c_str()
        );

        const auto& scope = m_Loaded[key] = factoryIt->second();
        return scope.get();
    }

    void SceneManager::LoadScene(std::string_view name) {
        while (!m_Stack.empty()) {
            m_Stack.back().second->OnUnload();
            AETHER_INFO("SceneManager: unloaded scene \"%s\"", m_Stack.back().first.c_str());

            m_Loaded.erase(m_Stack.back().first);
            m_Stack.pop_back();
        }

        Scene* scene = FindOrCreate(name);
        m_Stack.emplace_back(std::string(name), scene);
        scene->OnLoad();

        AETHER_INFO("SceneManager: loaded scene \"%s\"", std::string(name).c_str());
    }

    void SceneManager::PushScene(std::string_view name) {
        Scene* scene = FindOrCreate(name);
        m_Stack.emplace_back(std::string(name), scene);
        scene->OnLoad();

        AETHER_INFO(
            "SceneManager: pushed scene \"%s\" (stack depth: %zu)",
            std::string(name).c_str(), m_Stack.size()
        );
    }

    void SceneManager::PopScene() {
        if (m_Stack.empty()) {
            AETHER_WARN("SceneManager::PopScene — stack is already empty");
            return;
        }

        const auto& [name, scene] = m_Stack.back();
        scene->OnUnload();
        AETHER_INFO("SceneManager: popped scene \"%s\" (stack depth: %zu)", name.c_str(), m_Stack.size() - 1);

        m_Loaded.erase(name);
        m_Stack.pop_back();
    }

    void SceneManager::OnUpdate(float deltaTime) {
        if (!m_Stack.empty()) m_Stack.back().second->OnUpdate(deltaTime);
    }

    Scene* SceneManager::GetActiveScene() {
        return m_Stack.empty() ? nullptr : m_Stack.back().second;
    }

    const Scene* SceneManager::GetActiveScene() const {
        return m_Stack.empty() ? nullptr : m_Stack.back().second;
    }

    bool SceneManager::HasActiveScene() const {
        return !m_Stack.empty();
    }

    std::size_t SceneManager::StackDepth() const {
        return m_Stack.size();
    }
}
