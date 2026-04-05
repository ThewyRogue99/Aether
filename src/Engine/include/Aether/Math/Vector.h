//
// Created by ThewyRogue99 on 17.01.2026.
//

#pragma once

#include <cstddef>
#include <initializer_list>
#include <cassert>
#include <cmath>

namespace Aether::Math {
    template<typename T, size_t N>
    struct VectorStorage {
        T data[N];
    };

    template<typename T>
    struct VectorStorage<T, 2> {
        union {
            T data[2];
            struct { T x, y; };
            struct { T r, g; };
        };
    };

    template<typename T>
    struct VectorStorage<T, 3> {
        union {
            T data[3];
            struct { T x, y, z; };
            struct { T r, g, b; };
        };
    };

    template<typename T>
    struct VectorStorage<T, 4> {
        union {
            T data[4];
            struct { T x, y, z, w; };
            struct { T r, g, b, a; };
        };
    };

    template<typename T, size_t N>
    struct Vector : VectorStorage<T, N> {
        using Storage = VectorStorage<T, N>;

        constexpr Vector() = default;

        constexpr explicit Vector(T value) {
            for (size_t i = 0; i < N; ++i)
                this->data[i] = value;
        }

        constexpr Vector(std::initializer_list<T> init) {
            assert(init.size() == N);
            size_t i = 0;
            for (T v : init)
                this->data[i++] = v;
        }

        // Access
        constexpr T& operator[](size_t i) { return this->data[i]; }
        constexpr const T& operator[](size_t i) const { return this->data[i]; }

        // Arithmetic
        constexpr Vector operator+(const Vector& rhs) const {
            Vector out;
            for (size_t i = 0; i < N; ++i)
                out[i] = this->data[i] + rhs[i];
            return out;
        }

        constexpr Vector operator-(const Vector& rhs) const {
            Vector out;
            for (size_t i = 0; i < N; ++i)
                out[i] = this->data[i] - rhs[i];
            return out;
        }

        constexpr Vector operator*(T scalar) const {
            Vector out;
            for (size_t i = 0; i < N; ++i)
                out[i] = this->data[i] * scalar;
            return out;
        }

        constexpr Vector operator/(T scalar) const {
            Vector out;
            for (size_t i = 0; i < N; ++i)
                out[i] = this->data[i] / scalar;
            return out;
        }

        constexpr Vector& operator+=(const Vector& rhs) {
            for (size_t i = 0; i < N; ++i)
                this->data[i] += rhs[i];
            return *this;
        }

        constexpr Vector& operator-=(const Vector& rhs) {
            for (size_t i = 0; i < N; ++i)
                this->data[i] -= rhs[i];
            return *this;
        }

        constexpr Vector& operator*=(T scalar) {
            for (size_t i = 0; i < N; ++i)
                this->data[i] *= scalar;
            return *this;
        }

        constexpr Vector& operator/=(T scalar) {
            for (size_t i = 0; i < N; ++i)
                this->data[i] /= scalar;
            return *this;
        }

        // Dot product
        constexpr T Dot(const Vector& rhs) const {
            T sum{};
            for (size_t i = 0; i < N; ++i)
                sum += this->data[i] * rhs[i];
            return sum;
        }

        // Length
        T Length() const {
            return static_cast<T>(std::sqrt(Dot(*this)));
        }

        // Normalize
        Vector Normalized() const {
            T len = Length();
            assert(len != T{});
            return (*this) / len;
        }
    };

    // Scalar * Vector
    template<typename T, size_t N>
    constexpr Vector<T, N> operator*(T scalar, const Vector<T, N>& v) {
        return v * scalar;
    }

    using Vector2f = Vector<float, 2>;
    using Vector3f = Vector<float, 3>;
    using Vector4f = Vector<float, 4>;

    using Vector2d = Vector<double, 2>;
    using Vector3d = Vector<double, 3>;
    using Vector4d = Vector<double, 4>;
}
