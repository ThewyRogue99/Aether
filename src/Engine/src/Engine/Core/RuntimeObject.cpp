//
// Created by ThewyRogue99 on 31.12.2025.
//

#include <Aether/Core/RuntimeObject.h>

namespace Aether::Core {
    TypeId GetNextTypeId()
    {
        static TypeId counter = 0;
        return counter++;
    }
}
