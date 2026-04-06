//
// Created by ThewyRogue99 on 15.02.2026.
//

#pragma once

#include <typeindex>
#include <unordered_map>

#include <Aether/Core/Assert.h>
#include <Aether/ECS/EntityManager.h>
#include <Aether/ECS/ComponentStorage.h>

namespace Aether::ECS {
    class Registry {
    public:
        Entity CreateEntity();

        void DestroyEntity(const Entity& entity);

        [[nodiscard]] bool IsAlive(const Entity& entity) const;

        template<typename T, typename... Args>
        T& AddComponent(const Entity& entity, Args&&... args) {
            AETHER_ASSERT_MSG(IsAlive(entity), "Invalid entity!");

            auto& storage = GetStorage<T>();
            storage.Add(entity, T(std::forward<Args>(args)...));

            return storage.Get(entity);
        }

        template<typename T>
        void RemoveComponent(const Entity& entity) {
            AETHER_ASSERT_MSG(IsAlive(entity), "Invalid entity!");

            GetStorage<T>().Remove(entity);
        }

        template<typename T>
        T& GetComponent(const Entity& entity) {
            AETHER_ASSERT_MSG(IsAlive(entity), "Invalid entity!");

            return GetStorage<T>().Get(entity);
        }

        template<typename T>
        bool HasComponent(const Entity& entity) {
            if (!IsAlive(entity)) return false;

            return GetStorage<T>().Has(entity);
        }

    public:
        template<typename... Ts>
        class RegistryView {
        public:
            explicit RegistryView(ComponentStorage<Ts>*... storages) : m_Storages(storages...) {
                m_Primary = PickPrimaryStorage();
            }

            class Iterator {
            public:
                Iterator(const RegistryView* view, size_t index) : m_View(view), m_Index(index) {
                    SkipInvalid();
                }

                Entity operator*() const {
                    return m_View->PrimaryEntities()[m_Index];
                }

                Iterator& operator++() {
                    ++m_Index;
                    SkipInvalid();
                    return *this;
                }

                bool operator==(const Iterator& other) const {
                    return m_Index == other.m_Index && m_View == other.m_View;
                }

                bool operator!=(const Iterator& other) const {
                    return !(*this == other);
                }

            private:
                void SkipInvalid() {
                    const auto& ents = m_View->PrimaryEntities();

                    while (m_Index < ents.size() && !m_View->MatchesAll(ents[m_Index])) ++m_Index;
                }

                const RegistryView* m_View = nullptr;
                size_t m_Index = 0;
            };

            Iterator begin() const { return Iterator(this, 0); }
            Iterator end() const { return Iterator(this, PrimaryEntities().size()); }

            template<typename T>
            T& Get(Entity e) const {
                return Storage<T>().Get(e);
            }

        private:
            template<typename T>
            ComponentStorage<T>& Storage() const {
                return *std::get<ComponentStorage<T>*>(m_Storages);
            }

            [[nodiscard]] const void* PickPrimaryStorage() const {
                const void* best = nullptr;
                auto bestSize = static_cast<size_t>(-1);

                std::apply([&](auto*... s) {
                    (([&]
                    {
                        const size_t sz = s->Size();
                        if (sz < bestSize)
                        {
                            bestSize = sz;
                            best = static_cast<const void*>(s);
                        }
                    }()), ...);
                }, m_Storages);

                return best;
            }

            [[nodiscard]] const std::vector<Entity>& PrimaryEntities() const {
                const std::vector<Entity>* result = nullptr;

                std::apply([&](auto*... s) {
                    (([&]{
                        if (static_cast<const void*>(s) == m_Primary)
                            result = &s->Entities();
                    }()), ...);
                }, m_Storages);

                return *result;
            }

            [[nodiscard]] bool MatchesAll(Entity e) const {
                bool ok = true;
                std::apply([&](auto*... s) {
                    ((ok = ok && s->Has(e)), ...);
                }, m_Storages);

                return ok;
            }

        private:
            std::tuple<ComponentStorage<Ts>*...> m_Storages;
            const void* m_Primary = nullptr;
        };

        template<typename... Ts>
        RegistryView<Ts...> View() {
            AETHER_ASSERT_MSG(sizeof...(Ts) > 0, "ViewOf<T...> requires at least one component type.");

            return RegistryView<Ts...>(&GetStorage<Ts>()...);
        }

    private:
        template<typename T>
        ComponentStorage<T>& GetStorage() {
            const std::type_index type = typeid(T);

            if (!m_Storages.contains(type)) {
                m_Storages[type] = std::make_unique<ComponentStorage<T>>();
            }

            return *static_cast<ComponentStorage<T>*>(m_Storages[type].get());
        }

    private:
        EntityManager m_EntityManager;

        std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>> m_Storages;
    };
}
