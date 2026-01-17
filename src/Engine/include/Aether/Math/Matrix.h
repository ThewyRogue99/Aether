//
// Created by ThewyRogue99 on 17.01.2026.
//

#pragma once

#include <cstddef>
#include <initializer_list>
#include <cassert>

namespace Aether::Math {
    template<typename T, size_t R, size_t C>
    struct Matrix {
        T data[R][C]{};

        constexpr Matrix() = default;

        constexpr explicit Matrix(T value) {
            for (size_t r = 0; r < R; ++r)
                for (size_t c = 0; c < C; ++c)
                    data[r][c] = value;
        }

        constexpr Matrix(std::initializer_list<T> init) {
            assert(init.size() == R * C);
            size_t i = 0;
            for (T v : init)
            {
                data[i / C][i % C] = v;
                ++i;
            }
        }

        // Access
        constexpr T* operator[](size_t r) { return data[r]; }
        constexpr const T* operator[](size_t r) const { return data[r]; }

        // Identity (square only)
        static constexpr Matrix Identity() {
            static_assert(R == C);
            Matrix out{};
            for (size_t i = 0; i < R; ++i)
                out[i][i] = T{1};
            return out;
        }
    };

    // Matrix * Vector
    template<typename T, size_t R, size_t C>
    constexpr Vector<T, R> operator*(const Matrix<T, R, C>& m, const Vector<T, C>& v) {
        Vector<T, R> out{};
        for (size_t r = 0; r < R; ++r)
            for (size_t c = 0; c < C; ++c)
                out[r] += m[r][c] * v[c];
        return out;
    }

    // Matrix * Matrix
    template<typename T, size_t R, size_t C, size_t K>
    constexpr Matrix<T, R, K> operator*(const Matrix<T, R, C>& a, const Matrix<T, C, K>& b) {
        Matrix<T, R, K> out{};
        for (size_t r = 0; r < R; ++r)
            for (size_t k = 0; k < K; ++k)
                for (size_t c = 0; c < C; ++c)
                    out[r][k] += a[r][c] * b[c][k];
        return out;
    }

    using Matrix3f = Matrix<float, 3, 3>;
    using Matrix4f = Matrix<float, 4, 4>;
}
