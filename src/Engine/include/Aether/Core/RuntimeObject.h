//
// Created by ThewyRogue99 on 31.12.2025.
//

#pragma once

#include <cstdint>
#include <type_traits>

#include "Core.h"

namespace Aether::Engine {
    using TypeId = uint32_t;

    AETHER_API TypeId GetNextTypeId();

    class AETHER_API RuntimeObjectBase {
    public:
        virtual ~RuntimeObjectBase() = default;
        [[nodiscard]] virtual bool IsType(TypeId id) const = 0;
    };

    template<typename Derived, typename Parent = RuntimeObjectBase>
    class AETHER_API RuntimeObject : public Parent {
    public:
        static TypeId GetStaticTypeId() {
            static const TypeId id = GetNextTypeId();
            return id;
        }

        [[nodiscard]] bool IsType(TypeId id) const override {
            if (id == GetStaticTypeId())
                return true;

            if constexpr (std::is_base_of_v<RuntimeObjectBase, Parent>)
                return Parent::IsType(id);
            else
                return false;
        }
    };
}
