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

    Vector3f Normalize(const Vector3f& v);
    Vector3f Cross(const Vector3f& a, const Vector3f& b);

    Matrix4f Translate(const Matrix4f& m, const Vector3f& v);

    Matrix4f LookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up);

    Matrix4f Perspective(float fov, float aspect, float near, float far);
}
