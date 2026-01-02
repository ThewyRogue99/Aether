//
// Created by ThewyRogue99 on 31.12.2025.
//

#pragma once

#include "RuntimeObject.h"

namespace Aether::Core {
    template<typename T>
    concept RuntimeObjectType = std::is_base_of_v<RuntimeObjectBase, T>;

    template<RuntimeObjectType T>
    AETHER_API bool Is(const RuntimeObjectBase* obj) {
        return obj && obj->IsType(T::GetStaticTypeId());
    }

    template<RuntimeObjectType T>
    AETHER_API T* Cast(RuntimeObjectBase* obj) {
        return Is<T>(obj) ? static_cast<T*>(obj) : nullptr;
    }

    template<RuntimeObjectType T>
    AETHER_API const T* Cast(const RuntimeObjectBase* obj) {
        return Is<T>(obj) ? static_cast<const T*>(obj) : nullptr;
    }
}
