//
// Created by ThewyRogue99 on 03.05.2026.
//

#include <Aether/Core/Json.h>

#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

#include <Aether/Log/Log.h>

namespace Aether::Engine {
    class JsonValue::Impl {
    public:
        Impl() = default;
        explicit Impl(nlohmann::json j) : json(std::move(j)) {}

        nlohmann::json json;
    };

    JsonValue::JsonValue() = default;
    JsonValue::JsonValue(JsonValue&&) noexcept = default;
    JsonValue& JsonValue::operator=(JsonValue&&) noexcept = default;
    JsonValue::~JsonValue() = default;

    JsonValue::JsonValue(std::unique_ptr<Impl> impl)
        : m_Impl(std::move(impl)) {}

    JsonValue::JsonValue(const JsonValue& other) {
        if (other.m_Impl)
            m_Impl = std::make_unique<Impl>(other.m_Impl->json);
    }

    JsonValue& JsonValue::operator=(const JsonValue& other) {
        if (this != &other) {
            if (other.m_Impl)
                m_Impl = std::make_unique<Impl>(other.m_Impl->json);
            else
                m_Impl.reset();
        }
        return *this;
    }

    JsonValue JsonValue::Parse(Engine::StringView text) {
        try {
            auto j = nlohmann::json::parse(text.begin(), text.end());
            return JsonValue(std::make_unique<Impl>(std::move(j)));
        } catch (const nlohmann::json::exception& e) {
            AETHER_ERROR("JsonValue::Parse failed: %s", e.what());
            return JsonValue();
        }
    }

    JsonValue JsonValue::ParseFile(Engine::StringView path) {
        std::ifstream file{path.data()};
        if (!file.is_open()) {
            AETHER_ERROR("JsonValue::ParseFile - cannot open '%.*s'",
                         static_cast<int>(path.size()), path.data());
            return JsonValue();
        }
        std::ostringstream ss;
        ss << file.rdbuf();
        return Parse(ss.str().c_str());
    }

    bool JsonValue::IsNull()   const { return !m_Impl || m_Impl->json.is_null();    }
    bool JsonValue::IsBool()   const { return  m_Impl && m_Impl->json.is_boolean(); }
    bool JsonValue::IsNumber() const { return  m_Impl && m_Impl->json.is_number();  }
    bool JsonValue::IsString() const { return  m_Impl && m_Impl->json.is_string();  }
    bool JsonValue::IsArray()  const { return  m_Impl && m_Impl->json.is_array();   }
    bool JsonValue::IsObject() const { return  m_Impl && m_Impl->json.is_object();  }

    bool JsonValue::Contains(Engine::StringView key) const {
        if (!m_Impl || !m_Impl->json.is_object()) return false;
        return m_Impl->json.contains(key.data());
    }

    JsonValue JsonValue::At(Engine::StringView key) const {
        if (!m_Impl || !m_Impl->json.is_object()) return {};
        const auto it = m_Impl->json.find(key.data());
        if (it == m_Impl->json.end()) return {};
        return JsonValue(std::make_unique<Impl>(*it));
    }

    size_t JsonValue::Size() const {
        if (!m_Impl) return 0;
        return m_Impl->json.size();
    }

    JsonValue JsonValue::At(size_t index) const {
        if (!m_Impl || !m_Impl->json.is_array() || index >= m_Impl->json.size()) return {};
        return JsonValue(std::make_unique<Impl>(m_Impl->json[index]));
    }

    void JsonValue::ForEach(const std::function<void(const JsonValue&)>& fn) const {
        if (!m_Impl || !m_Impl->json.is_array()) return;
        for (const auto& elem : m_Impl->json) {
            JsonValue v(std::make_unique<Impl>(elem));
            fn(v);
        }
    }

    bool JsonValue::AsBool(bool fallback) const {
        if (!m_Impl || !m_Impl->json.is_boolean()) return fallback;
        return m_Impl->json.get<bool>();
    }

    int64_t JsonValue::AsInt(int64_t fallback) const {
        if (!m_Impl || !m_Impl->json.is_number()) return fallback;
        return m_Impl->json.get<int64_t>();
    }

    double JsonValue::AsFloat(double fallback) const {
        if (!m_Impl || !m_Impl->json.is_number()) return fallback;
        return m_Impl->json.get<double>();
    }

    Engine::String JsonValue::AsString(Engine::StringView fallback) const {
        if (!m_Impl || !m_Impl->json.is_string()) return Engine::String(fallback);
        return m_Impl->json.get<std::string>().c_str();
    }
} // namespace Aether::Engine
