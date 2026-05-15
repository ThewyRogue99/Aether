//
// Created by ThewyRogue99 on 03.05.2026.
//

#pragma once

#include <functional>
#include <memory>

#include <Aether/Core/Core.h>
#include <Aether/Core/String.h>
#include <Aether/Core/StringView.h>

namespace Aether::Engine {
    class AETHER_API JsonValue {
    public:
        JsonValue();
        JsonValue(const JsonValue& other);
        JsonValue(JsonValue&& other) noexcept;
        JsonValue& operator=(const JsonValue& other);
        JsonValue& operator=(JsonValue&& other) noexcept;
        ~JsonValue();

        // ── Parsing ──────────────────────────────────────────────────────────────
        // Returns a null JsonValue and logs on failure.

        [[nodiscard]] static JsonValue Parse(Engine::StringView text);
        [[nodiscard]] static JsonValue ParseFile(Engine::StringView path);

        // ── Type checks ──────────────────────────────────────────────────────────

        [[nodiscard]] bool IsNull() const;
        [[nodiscard]] bool IsBool() const;
        [[nodiscard]] bool IsNumber() const;
        [[nodiscard]] bool IsString() const;
        [[nodiscard]] bool IsArray() const;
        [[nodiscard]] bool IsObject() const;

        // ── Object access ────────────────────────────────────────────────────────

        [[nodiscard]] bool Contains(Engine::StringView key) const;
        [[nodiscard]] JsonValue At(Engine::StringView key) const;

        // ── Array access ─────────────────────────────────────────────────────────

        [[nodiscard]] size_t Size() const;
        [[nodiscard]] JsonValue At(size_t index) const;
        void ForEach(const std::function<void(const JsonValue&)>& fn) const;

        // ── Typed accessors ──────────────────────────────────────────────────────

        [[nodiscard]] bool AsBool(bool fallback = false) const;
        [[nodiscard]] int64_t AsInt(int64_t fallback = 0) const;
        [[nodiscard]] double AsFloat(double fallback = 0.0) const;
        [[nodiscard]] Engine::String AsString(Engine::StringView fallback = {}) const;

    private:
        class Impl;
        std::unique_ptr<Impl> m_Impl;

        explicit JsonValue(std::unique_ptr<Impl> impl);
    };
} // namespace Aether::Engine
