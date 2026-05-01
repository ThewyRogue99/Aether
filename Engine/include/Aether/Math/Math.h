//
// Created by ThewyRogue99 on 17.01.2026.
//

#pragma once

#include <Aether/Math/Matrix.h>
#include <Aether/Math/Vector.h>

namespace Aether::Math {
    float Radians(float degrees);
    float Degrees(float radians);

    template<typename T, size_t N>
    constexpr Vector<T, N> Radians(const Vector<T, N>& degrees) {
        Vector<T, N> result;
        for (size_t i = 0; i < N; ++i) result[i] = Radians(degrees[i]);
        return result;
    }

    template<typename T, size_t N>
    constexpr Vector<T, N> Degrees(const Vector<T, N>& radians) {
        Vector<T, N> result;
        for (size_t i = 0; i < N; ++i) result[i] = Degrees(radians[i]);
        return result;
    }

    template<typename T>
    constexpr Matrix<T, 4, 4> Translate(const Matrix<T, 4, 4>& m, const Vector<T, 3>& v) {
        Matrix<T, 4, 4> Result(m);
        Result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
        return Result;
    }

    template<typename T>
    Matrix<T, 4, 4> Rotate(const Matrix<T, 4, 4>& m, T angle, const Vector<T, 3>& v) {
        T const a = angle;
        T const c = cos(a);
        T const s = sin(a);

        Vector<T, 3> axis(Normalize(v));
        Vector<T, 3> temp((T(1) - c) * axis);

        Matrix<T, 4, 4> Rotate;
        Rotate[0][0] = c + temp[0] * axis[0];
        Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
        Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

        Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
        Rotate[1][1] = c + temp[1] * axis[1];
        Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

        Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
        Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
        Rotate[2][2] = c + temp[2] * axis[2];

        Matrix<T, 4, 4> Result;
        Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
        Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
        Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
        Result[3] = m[3];
        return Result;
    }

    template<typename T>
    Matrix<T, 4, 4> Scale(const Matrix<T, 4, 4>& m, const Vector<T, 3>& v) {
        Matrix<T, 4, 4> Result;
        Result[0] = m[0] * v[0];
        Result[1] = m[1] * v[1];
        Result[2] = m[2] * v[2];
        Result[3] = m[3];
        return Result;
    }

    Vector3f Normalize(const Vector3f& v);
    Vector3f Cross(const Vector3f& a, const Vector3f& b);

    Matrix4f LookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up);

    Matrix4f Perspective(float fov, float aspect, float near, float far);
}
