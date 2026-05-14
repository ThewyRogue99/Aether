//
// Created by ThewyRogue99 on 05.05.2026.
//

#pragma once

#include <cstdint>

#include <Aether/Core/Core.h>
#include <Aether/Core/String.h>
#include <Aether/Math/Vector.h>

namespace Aether::Serialization {
    class AETHER_API Archive {
    public:
        virtual ~Archive() = default;

        [[nodiscard]] virtual bool IsReading() const = 0;
        [[nodiscard]] virtual bool IsWriting() const = 0;

        // ── Primitives ─────────────────────────────────────────────────────────────
        virtual void operator()(Engine::StringView key, bool& value) = 0;
        virtual void operator()(Engine::StringView key, int32_t& value) = 0;
        virtual void operator()(Engine::StringView key, uint32_t& value) = 0;
        virtual void operator()(Engine::StringView key, float& value) = 0;
        virtual void operator()(Engine::StringView key, Engine::String& value) = 0;

        virtual void BeginObject(Engine::StringView key) = 0;
        virtual void EndObject() = 0;

        virtual void BeginArray(Engine::StringView key) = 0;
        virtual size_t ArraySize() = 0;
        virtual void BeginArrayElement() = 0;
        virtual void EndArrayElement() = 0;
        virtual void EndArray() = 0;

        // Returns true when the current object context contains the given key.
        [[nodiscard]] virtual bool HasKey(Engine::StringView key) const { return false; }

        void operator()(Engine::StringView key, Math::Vector2f& v) {
            BeginObject(key);
            (*this)("x", v.x);
            (*this)("y", v.y);
            EndObject();
        }

        void operator()(Engine::StringView key, Math::Vector3f& v) {
            BeginObject(key);
            (*this)("x", v.x);
            (*this)("y", v.y);
            (*this)("z", v.z);
            EndObject();
        }

        void operator()(Engine::StringView key, Math::Vector4f& v) {
            BeginObject(key);
            (*this)("x", v.x);
            (*this)("y", v.y);
            (*this)("z", v.z);
            (*this)("w", v.w);
            EndObject();
        }

        template<typename T>
        void Object(Engine::StringView key, T& obj) {
            BeginObject(key);
            obj.Serialize(*this);
            EndObject();
        }
    };
} // namespace Aether::Engine
