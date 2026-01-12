//
// Created by ThewyRogue99 on 1.01.2026.
//

#include "TempScope.h"

#include "TempAllocator.h"

#include <Aether/Core/Memory/LinearAllocator.h>

namespace Aether::Engine {
    class TempScope::Impl {
    public:
        Impl() : m_alloc(TempAllocator()) , m_marker(m_alloc.mark()) { }
        ~Impl() { m_alloc.rewind(m_marker); }

        LinearAllocator& m_alloc;
        LinearAllocator::Marker m_marker;
    };

    TempScope::TempScope() : m_impl(MakeScope<Impl>()) { }

    TempScope::~TempScope() = default;
}
