//
// Created by ThewyRogue99 on 05.05.2026.
//

#pragma once

#include <memory>

#include <Aether/Core/Core.h>
#include <Aether/Core/Serialization/Archive.h>

namespace Aether::Serialization {
    // Builds a JSON document as Serialize() calls are made.
    // Retrieve the result with ToString() or write it directly with SaveToFile().
    class AETHER_API JsonWriteArchive : public Archive {
    public:
        JsonWriteArchive();
        ~JsonWriteArchive() override;

        [[nodiscard]] bool IsReading() const override { return false; }
        [[nodiscard]] bool IsWriting() const override { return true; }

        // ── Output ─────────────────────────────────────────────────────────────────

        [[nodiscard]] Engine::String ToString() const;
        [[nodiscard]] bool SaveToFile(Engine::StringView path) const;

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
        size_t ArraySize() override { return 0; }
        void BeginArrayElement() override;
        void EndArrayElement() override;
        void EndArray() override;

    private:
        class Impl;
        std::unique_ptr<Impl> m_Impl;
    };

} // namespace Aether::Engine
