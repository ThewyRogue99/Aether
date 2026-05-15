// BasicString.inl — inline template definitions.
// Included at the bottom of BasicString.h; never #include this directly.
// DefaultAllocator() is declared in <Aether/Core/Memory/BasicAllocator.h>,
// which BasicString.h already includes.

#pragma once

namespace Aether::Engine {
    // ── Construction / lifetime ───────────────────────────────────────────────────

    template<typename CharT>
    BasicString<CharT>::BasicString() = default;

    template<typename CharT>
    BasicString<CharT>::BasicString(const BasicString& other)
        : m_alloc(other.m_alloc ? other.m_alloc : &DefaultAllocator()) {
        assign(other.view());
    }

    template<typename CharT>
    BasicString<CharT>::BasicString(BasicString&& other) noexcept {
        moveFrom(std::move(other));
    }

    template<typename CharT>
    BasicString<CharT>::BasicString(ViewType sv, BasicAllocator* alloc)
        : m_alloc(alloc ? alloc : &DefaultAllocator()) {
        assign(sv);
    }

    template<typename CharT>
    BasicString<CharT>::BasicString(const CharT* cstr, BasicAllocator* alloc)
        : m_alloc(alloc ? alloc : &DefaultAllocator()) {
        assign(ViewType::FromCString(cstr));
    }

    template<typename CharT>
    BasicString<CharT>::~BasicString() { release(); }

    template<typename CharT>
    BasicString<CharT>& BasicString<CharT>::operator=(const BasicString& other) {
        if (this == &other) return *this;
        if (!m_alloc) m_alloc = other.m_alloc ? other.m_alloc : &DefaultAllocator();
        assign(other.view());
        return *this;
    }

    template<typename CharT>
    BasicString<CharT>& BasicString<CharT>::operator=(BasicString&& other) noexcept {
        if (this == &other) return *this;
        release();
        moveFrom(std::move(other));
        return *this;
    }

    // ── Capacity ──────────────────────────────────────────────────────────────────

    template<typename CharT> uint32_t BasicString<CharT>::size() const { return m_size; }
    template<typename CharT> uint32_t BasicString<CharT>::length() const { return m_size; }
    template<typename CharT> uint32_t BasicString<CharT>::capacity() const { return m_capacity; }
    template<typename CharT> bool BasicString<CharT>::empty() const { return m_size == 0; }

    template<typename CharT>
    void BasicString<CharT>::reserve(uint32_t newCap) {
        if (newCap <= m_capacity) return;
        growTo(newCap);
    }

    // ── Element access ────────────────────────────────────────────────────────────

    template<typename CharT>
    const CharT* BasicString<CharT>::c_str() const {
        // Return a pointer to a static null for empty strings so c_str() is always valid.
        static constexpr CharT kNull = CharT{};
        return m_data ? m_data : &kNull;
    }

    template<typename CharT> const CharT* BasicString<CharT>::data() const { return c_str(); }
    template<typename CharT> CharT* BasicString<CharT>::data() { return m_data; }

    template<typename CharT>
    CharT BasicString<CharT>::operator[](uint32_t i) const {
        AETHER_ASSERT(i < m_size);
        return m_data[i];
    }
    template<typename CharT>
    CharT& BasicString<CharT>::operator[](uint32_t i) {
        AETHER_ASSERT(i < m_size);
        return m_data[i];
    }

    template<typename CharT>
    CharT BasicString<CharT>::at(uint32_t i) const {
        AETHER_ASSERT_MSG(i < m_size, "BasicString::at out of range");
        return m_data[i];
    }

    template<typename CharT>
    CharT& BasicString<CharT>::at(uint32_t i) {
        AETHER_ASSERT_MSG(i < m_size, "BasicString::at out of range");
        return m_data[i];
    }

    template<typename CharT>
    CharT BasicString<CharT>::front() const {
        AETHER_ASSERT_MSG(m_size > 0, "BasicString::front on empty string");
        return m_data[0];
    }

    template<typename CharT>
    CharT& BasicString<CharT>::front() {
        AETHER_ASSERT_MSG(m_size > 0, "BasicString::front on empty string");
        return m_data[0];
    }

    template<typename CharT>
    CharT BasicString<CharT>::back() const {
        AETHER_ASSERT_MSG(m_size > 0, "BasicString::back on empty string");
        return m_data[m_size - 1];
    }

    template<typename CharT>
    CharT& BasicString<CharT>::back() {
        AETHER_ASSERT_MSG(m_size > 0, "BasicString::back on empty string");
        return m_data[m_size - 1];
    }

    // ── Iterators ─────────────────────────────────────────────────────────────────

    template<typename CharT> const CharT* BasicString<CharT>::begin() const { return m_data; }
    template<typename CharT> CharT* BasicString<CharT>::begin() { return m_data; }
    template<typename CharT> const CharT* BasicString<CharT>::end() const { return m_data ? m_data + m_size : nullptr; }
    template<typename CharT> CharT* BasicString<CharT>::end() { return m_data ? m_data + m_size : nullptr; }
    template<typename CharT> const CharT* BasicString<CharT>::cbegin() const { return m_data; }
    template<typename CharT> const CharT* BasicString<CharT>::cend() const { return m_data ? m_data + m_size : nullptr; }

    // ── Conversion ────────────────────────────────────────────────────────────────

    template<typename CharT>
    BasicStringView<CharT> BasicString<CharT>::view() const {
        return ViewType(data(), m_size);
    }

    // ── Modifiers ─────────────────────────────────────────────────────────────────

    template<typename CharT>
    void BasicString<CharT>::clear() {
        m_size = 0;
        if (m_data) m_data[0] = CharT{};
    }

    template<typename CharT>
    void BasicString<CharT>::assign(ViewType sv) {
        clear();
        reserve(sv.size());
        if (!sv.empty()) {
            Traits::copy(m_data, sv.data(), sv.size());
            m_size = sv.size();
            m_data[m_size] = CharT{};
        }
    }

    template<typename CharT>
    void BasicString<CharT>::append(ViewType sv) {
        if (sv.empty()) return;
        const uint32_t required = m_size + sv.size();
        if (required > m_capacity) {
            uint32_t newCap = m_capacity ? (m_capacity + m_capacity / 2) : 16u;
            if (newCap < required) newCap = required;
            growTo(newCap);
        }
        Traits::copy(m_data + m_size, sv.data(), sv.size());
        m_size += sv.size();
        m_data[m_size] = CharT{};
    }

    template<typename CharT>
    void BasicString<CharT>::push_back(CharT c) {
        if (m_size + 1 > m_capacity) {
            uint32_t newCap = m_capacity ? (m_capacity + m_capacity / 2) : 16u;
            if (newCap < m_size + 1) newCap = m_size + 1;
            growTo(newCap);
        }
        m_data[m_size++] = c;
        m_data[m_size]   = CharT{};
    }

    template<typename CharT>
    void BasicString<CharT>::pop_back() {
        AETHER_ASSERT_MSG(m_size > 0, "BasicString::pop_back on empty string");
        --m_size;
        m_data[m_size] = CharT{};
    }

    template<typename CharT>
    void BasicString<CharT>::resize(uint32_t newSize) { resize(newSize, CharT{}); }

    template<typename CharT>
    void BasicString<CharT>::resize(uint32_t newSize, CharT fill) {
        if (newSize == m_size) return;
        if (newSize < m_size) {
            m_size = newSize;
            if (m_data) m_data[m_size] = CharT{};
            return;
        }
        if (newSize > m_capacity) growTo(newSize);
        for (uint32_t i = m_size; i < newSize; ++i) m_data[i] = fill;
        m_size = newSize;
        m_data[m_size] = CharT{};
    }

    template<typename CharT>
    void BasicString<CharT>::erase(uint32_t pos, uint32_t count) {
        AETHER_ASSERT_MSG(pos <= m_size, "BasicString::erase pos out of range");
        const uint32_t available = m_size - pos;
        const uint32_t toRemove = (count < available) ? count : available;
        if (toRemove == 0) return;

        const uint32_t tailStart = pos + toRemove;
        const uint32_t tailLen = m_size - tailStart;
        if (tailLen > 0)
            Traits::move(m_data + pos, m_data + tailStart, tailLen);

        m_size -= toRemove;
        m_data[m_size] = CharT{};
    }

    template<typename CharT>
    BasicString<CharT>& BasicString<CharT>::operator+=(ViewType sv) {
        append(sv);
        return *this;
    }

    template<typename CharT>
    BasicString<CharT>& BasicString<CharT>::operator+=(CharT c) {
        push_back(c);
        return *this;
    }

    template<typename CharT>
    BasicString<CharT>& BasicString<CharT>::operator+=(const CharT* cstr) {
        append(ViewType::FromCString(cstr));
        return *this;
    }

    // ── Substring / search ────────────────────────────────────────────────────────

    template<typename CharT>
    BasicString<CharT> BasicString<CharT>::substr(uint32_t pos, uint32_t count) const {
        return BasicString(view().substr(pos, count), m_alloc);
    }

    template<typename CharT> uint32_t BasicString<CharT>::find(CharT c, uint32_t pos) const { return view().find(c, pos); }
    template<typename CharT> uint32_t BasicString<CharT>::find(ViewType sv, uint32_t pos) const { return view().find(sv, pos); }
    template<typename CharT> uint32_t BasicString<CharT>::rfind(CharT c, uint32_t pos) const { return view().rfind(c, pos); }
    template<typename CharT> uint32_t BasicString<CharT>::rfind(ViewType sv, uint32_t pos) const { return view().rfind(sv, pos); }

    template<typename CharT> bool BasicString<CharT>::starts_with(ViewType prefix) const { return view().starts_with(prefix); }
    template<typename CharT> bool BasicString<CharT>::starts_with(CharT c) const { return view().starts_with(c); }
    template<typename CharT> bool BasicString<CharT>::ends_with(ViewType suffix) const { return view().ends_with(suffix); }
    template<typename CharT> bool BasicString<CharT>::ends_with(CharT c) const { return view().ends_with(c); }
    template<typename CharT> bool BasicString<CharT>::contains(ViewType needle) const { return view().contains(needle); }
    template<typename CharT> bool BasicString<CharT>::contains(CharT c) const { return view().contains(c); }

    // ── Comparison ────────────────────────────────────────────────────────────────

    template<typename CharT>
    int BasicString<CharT>::compare(ViewType other) const { return view().compare(other); }

    template<typename CharT>
    bool BasicString<CharT>::operator==(const BasicString& other) const {
        if (m_size != other.m_size) return false;
        if (m_size == 0) return true;
        return Traits::compare(m_data, other.m_data, m_size) == 0;
    }

    template<typename CharT>
    bool BasicString<CharT>::operator!=(const BasicString& other) const { return !(*this == other); }

    template<typename CharT>
    bool BasicString<CharT>::operator<(const BasicString& other) const {
        const uint32_t minSize = (m_size < other.m_size) ? m_size : other.m_size;
        if (minSize > 0) {
            const int cmp = Traits::compare(m_data, other.m_data, minSize);
            if (cmp != 0) return cmp < 0;
        }
        return m_size < other.m_size;
    }

    template<typename CharT>
    bool BasicString<CharT>::operator==(ViewType other) const {
        if (m_size != other.size()) return false;
        if (m_size == 0) return true;
        return Traits::compare(m_data, other.data(), m_size) == 0;
    }
    template<typename CharT>
    bool BasicString<CharT>::operator!=(ViewType other) const { return !(*this == other); }

    template<typename CharT>
    bool BasicString<CharT>::operator==(const CharT* cstr) const { return *this == ViewType::FromCString(cstr); }
    template<typename CharT>
    bool BasicString<CharT>::operator!=(const CharT* cstr) const { return !(*this == cstr); }

    // ── Internal helpers ──────────────────────────────────────────────────────────

    template<typename CharT>
    void BasicString<CharT>::release() {
        if (m_data && m_alloc) m_alloc->deallocate(m_data);

        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
    }

    template<typename CharT>
    void BasicString<CharT>::moveFrom(BasicString&& other) noexcept {
        m_alloc = other.m_alloc;
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_alloc = nullptr;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    template<typename CharT>
    void BasicString<CharT>::growTo(uint32_t newCap) {
        if (!m_alloc) m_alloc = &DefaultAllocator();

        // Allocate in bytes; each slot is sizeof(CharT) bytes.
        const std::size_t bytes = (static_cast<std::size_t>(newCap) + 1) * sizeof(CharT);
        auto* newData = static_cast<CharT*>(m_alloc->allocate(bytes));
        AETHER_ASSERT(newData);

        if (m_data && m_size) Traits::copy(newData, m_data, m_size);
        newData[m_size] = CharT{};

        if (m_data) m_alloc->deallocate(m_data);
        m_data = newData;
        m_capacity = newCap;
    }
} // namespace Aether::Engine
