//
// Created by ThewyRogue99 on 1.01.2026.
//

#include <Aether/Core/Name.h>

#include <mutex>
#include <vector>
#include <unordered_map>

#include <Aether/Core/Hash.h>
#include <Aether/Core/Assert.h>
#include <Aether/Core/StringView.h>
#include <Aether/Core/Memory/LinearAllocator.h>

#include "Memory/NameAllocator.h"

namespace Aether::Engine {
    class Name::Impl {
    public:
        Impl() = default;

        explicit Impl(const StringView& sv) : m_id(Intern(sv)) {}
        explicit Impl(const char* cstr) : m_id(Intern(StringView::FromCString(cstr))) { }

        class Table {
        public:
            std::mutex mtx;

            std::vector<StringView> strings;
            std::unordered_map<uint32_t, std::vector<uint32_t>> hashToIds;

            uint32_t intern(StringView& sv) {
                const uint32_t h = FNV1a32(sv);

                std::scoped_lock lock(mtx);

                auto& bucket = hashToIds[h];
                for (uint32_t id : bucket) {
                    const StringView& s = strings[id];
                    if (s.size() == sv.size() && std::memcmp(s.data(), sv.data(), sv.size()) == 0) {
                        return id;
                    }
                }

                // new string
                const uint32_t newId = static_cast<uint32_t>(strings.size());
                char* mem = static_cast<char*>(NameAllocator().allocate(sv.size() + 1, alignof(char)));
                std::memcpy(mem, sv.data(), sv.size());
                mem[sv.size()] = '\0';

                strings.push_back(StringView{ mem, sv.size() });
                bucket.push_back(newId);
                return newId;
            }

            StringView toString(uint32_t id) {
                std::scoped_lock lock(mtx);
                if (id == 0) return {};
                AETHER_ASSERT(id < strings.size());
                return strings[id];
            }

            Table() {
                // reserve id=0 as "none"
                strings.emplace_back(StringView{ "", 0 });
            }
        };

        static Table& NameTable() {
            static Table t;
            return t;
        }

        static uint32_t Intern(StringView sv) {
            if (sv.empty()) return 0;
            return NameTable().intern(sv);
        }

        uint32_t m_id = 0;
    };

    Name::Name() : m_impl(MakeScope<Impl>()) { };

    Name::Name(StringView& sv) : m_impl(MakeScope<Impl>(sv)) { };
    Name::Name(const char* cstr) : m_impl(MakeScope<Impl>(cstr)) { };

    uint32_t Name::id() const { return m_impl->m_id; }
    Name::operator bool() const { return m_impl->m_id != 0; }

    bool operator==(const Name &a, const Name &b) { return a.m_impl->m_id == b.m_impl->m_id; }
    bool operator!=(const Name &a, const Name &b) { return a.m_impl->m_id != b.m_impl->m_id; }

    // Debug/Editor only: get original string (thread-safe)
    String Name::ToString(const Name &n) {
        const StringView sv = Impl::NameTable().toString(n.m_impl->m_id);

        return String(sv);
    }
}
