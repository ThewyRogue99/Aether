//
// Created by ThewyRogue99 on 05.05.2026.
//

#include <Aether/Core/Serialization/JsonReadArchive.h>

#include <vector>

#include <Aether/Core/Json.h>
#include <Aether/Log/Log.h>

namespace Aether::Serialization {
    class JsonReadArchive::Impl {
    public:
        Impl() = default;

        Engine::JsonValue root;
        bool valid = false;

        // Each frame on the stack represents a JSON node being traversed.
        // arrayIndex is only used when the frame is an array.
        struct Frame {
            Engine::JsonValue value;
            size_t arrayIndex = 0;
        };

        std::vector<Frame> stack;

        [[nodiscard]] const Engine::JsonValue& Current() const {
            return stack.empty() ? root : stack.back().value;
        }
    };

    JsonReadArchive::JsonReadArchive()
        : m_Impl(std::make_unique<Impl>()) {}

    JsonReadArchive::JsonReadArchive(std::unique_ptr<Impl> impl)
        : m_Impl(std::move(impl)) {}

    JsonReadArchive::~JsonReadArchive() = default;

    JsonReadArchive::JsonReadArchive(JsonReadArchive&&) noexcept = default;
    JsonReadArchive& JsonReadArchive::operator=(JsonReadArchive&&) noexcept = default;

    JsonReadArchive JsonReadArchive::FromFile(Engine::StringView path) {
        auto impl = std::make_unique<Impl>();

        impl->root = Engine::JsonValue::ParseFile(path);
        impl->valid = !impl->root.IsNull();

        if (!impl->valid) {
            AETHER_ERROR(
                "JsonReadArchive: failed to parse '%.*s'",
                static_cast<int>(path.size()),
                path.data()
            );
        }

        return JsonReadArchive(std::move(impl));
    }

    JsonReadArchive JsonReadArchive::FromString(Engine::StringView json) {
        auto impl = std::make_unique<Impl>();

        impl->root = Engine::JsonValue::Parse(json);
        impl->valid = !impl->root.IsNull();

        if (!impl->valid) AETHER_ERROR("JsonReadArchive: failed to parse JSON string");

        return JsonReadArchive(std::move(impl));
    }

    bool JsonReadArchive::IsValid() const {
        return m_Impl && m_Impl->valid;
    }

    void JsonReadArchive::operator()(Engine::StringView key, bool& value) {
        value = m_Impl->Current().At(key).AsBool(value);
    }

    void JsonReadArchive::operator()(Engine::StringView key, int32_t& value) {
        value = static_cast<int32_t>(m_Impl->Current().At(key).AsInt(value));
    }

    void JsonReadArchive::operator()(Engine::StringView key, uint32_t& value) {
        value = static_cast<uint32_t>(m_Impl->Current().At(key).AsInt(value));
    }

    void JsonReadArchive::operator()(Engine::StringView key, float& value) {
        value = static_cast<float>(m_Impl->Current().At(key).AsFloat(value));
    }

    void JsonReadArchive::operator()(Engine::StringView key, Engine::String& value) {
        value = m_Impl->Current().At(key).AsString(value);
    }

    void JsonReadArchive::BeginObject(Engine::StringView key) {
        m_Impl->stack.push_back({ m_Impl->Current().At(key), 0 });
    }

    void JsonReadArchive::EndObject() {
        m_Impl->stack.pop_back();
    }

    void JsonReadArchive::BeginArray(Engine::StringView key) {
        m_Impl->stack.push_back({ m_Impl->Current().At(key), 0 });
    }

    size_t JsonReadArchive::ArraySize() {
        return m_Impl->stack.back().value.Size();
    }

    void JsonReadArchive::BeginArrayElement() {
        // Grab the current index from the array frame, then advance it.
        const size_t index = m_Impl->stack.back().arrayIndex++;
        Engine::JsonValue elem = m_Impl->stack.back().value.At(index);
        m_Impl->stack.push_back({ std::move(elem), 0 });
    }

    void JsonReadArchive::EndArrayElement() {
        m_Impl->stack.pop_back();
    }

    void JsonReadArchive::EndArray() {
        m_Impl->stack.pop_back();
    }

    bool JsonReadArchive::HasKey(Engine::StringView key) const {
        return m_Impl->Current().Contains(key);
    }
} // namespace Aether::Engine
