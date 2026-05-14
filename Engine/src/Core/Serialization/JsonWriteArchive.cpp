//
// Created by ThewyRogue99 on 05.05.2026.
//

#include <Aether/Core/Serialization/JsonWriteArchive.h>

#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>

#include <Aether/Log/Log.h>

namespace Aether::Serialization {
    class JsonWriteArchive::Impl {
    public:
        Impl() = default;

        // Each stack frame owns a JSON value being built and knows how to commit
        // itself into the parent when End*() is called.
        struct Frame {
            nlohmann::json value;
            std::string keyInParent;    // key used when parent is an object
            bool isArrayElement; // true → push_back into parent array
        };

        nlohmann::json root = nlohmann::json::object();
        std::vector<Frame> stack;

        nlohmann::json& Current() {
            return stack.empty() ? root : stack.back().value;
        }

        // Pop the top frame and commit its value into the new top (or root).
        void Commit() {
            Frame frame = std::move(stack.back());
            stack.pop_back();

            auto& parent = stack.empty() ? root : stack.back().value;
            if (frame.isArrayElement)
                parent.push_back(std::move(frame.value));
            else
                parent[frame.keyInParent] = std::move(frame.value);
        }
    };

    JsonWriteArchive::JsonWriteArchive()
        : m_Impl(std::make_unique<Impl>()) {}

    JsonWriteArchive::~JsonWriteArchive() = default;

    Engine::String JsonWriteArchive::ToString() const {
        return m_Impl->root.dump(4).c_str();
    }

    bool JsonWriteArchive::SaveToFile(Engine::StringView path) const {
        std::ofstream file{std::string(path)};
        if (!file.is_open()) {
            AETHER_ERROR("JsonWriteArchive: cannot open '%.*s' for writing",
                         static_cast<int>(path.size()), path.data());
            return false;
        }
        file << m_Impl->root.dump(4);
        return true;
    }

    void JsonWriteArchive::operator()(Engine::StringView key, bool& value) {
        m_Impl->Current()[std::string(key)] = value;
    }

    void JsonWriteArchive::operator()(Engine::StringView key, int32_t& value) {
        m_Impl->Current()[std::string(key)] = value;
    }

    void JsonWriteArchive::operator()(Engine::StringView key, uint32_t& value) {
        m_Impl->Current()[std::string(key)] = value;
    }

    void JsonWriteArchive::operator()(Engine::StringView key, float& value) {
        m_Impl->Current()[std::string(key)] = value;
    }

    void JsonWriteArchive::operator()(Engine::StringView key, Engine::String& value) {
        m_Impl->Current()[std::string(key)] = value;
    }

    void JsonWriteArchive::BeginObject(Engine::StringView key) {
        m_Impl->stack.push_back({ nlohmann::json::object(), std::string(key), false });
    }

    void JsonWriteArchive::EndObject() {
        m_Impl->Commit();
    }

    void JsonWriteArchive::BeginArray(Engine::StringView key) {
        m_Impl->stack.push_back({ nlohmann::json::array(), std::string(key), false });
    }

    void JsonWriteArchive::BeginArrayElement() {
        // Array element: an object that will be push_back'd into the parent array.
        m_Impl->stack.push_back({ nlohmann::json::object(), {}, true });
    }

    void JsonWriteArchive::EndArrayElement() {
        m_Impl->Commit();
    }

    void JsonWriteArchive::EndArray() {
        m_Impl->Commit();
    }
} // namespace Aether::Engine
