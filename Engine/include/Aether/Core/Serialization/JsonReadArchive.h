//
// Created by ThewyRogue99 on 05.05.2026.
//

#pragma once

#include <memory>

#include <Aether/Core/Core.h>
#include <Aether/Core/Serialization/Archive.h>

namespace Aether::Serialization {
    // Reads values from a JSON document into objects as Serialize() calls are made.
    // Construct with FromFile() or FromString(); check IsValid() before using.
    class AETHER_API JsonReadArchive : public Archive {
    public:
        static JsonReadArchive FromFile(Engine::StringView path);
        static JsonReadArchive FromString(Engine::StringView json);

        JsonReadArchive();

        ~JsonReadArchive() override;

        JsonReadArchive(JsonReadArchive&&) noexcept;
        JsonReadArchive& operator=(JsonReadArchive&&) noexcept;

        // False if the source JSON failed to parse.
        [[nodiscard]] bool IsValid() const;

        [[nodiscard]] bool IsReading() const override { return true; }
        [[nodiscard]] bool IsWriting() const override { return false; }

        // ── Primitives ─────────────────────────────────────────────────────────────

        void operator()(Engine::StringView key, bool& value) override;
        void operator()(Engine::StringView key, int32_t& value) override;
        void operator()(Engine::StringView key, uint32_t& value) override;
        void operator()(Engine::StringView key, float& value) override;
        void operator()(Engine::StringView key, Engine::String& value) override;

        // ── Structured traversal ───────────────────────────────────────────────────

        void BeginObject(Engine::StringView key) override;
        void EndObject() override;
        void BeginArray(Engine::StringView key) override;
        size_t ArraySize() override;
        void BeginArrayElement() override;
        void EndArrayElement() override;
        void EndArray() override;

        [[nodiscard]] bool HasKey(Engine::StringView key) const override;

    private:
        class Impl;
        std::unique_ptr<Impl> m_Impl;

        explicit JsonReadArchive(std::unique_ptr<Impl> impl);
    };
} // namespace Aether::Engine
