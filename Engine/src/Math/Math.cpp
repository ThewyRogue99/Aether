//
// Created by ThewyRogue99 on 17.01.2026.
//

#include <Aether/Math/Math.h>

#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Interop.h"

namespace Aether::Math {
    float Radians(float degrees) {
        return glm::radians(degrees);
    }

    float Degrees(float radians) {
        return glm::degrees(radians);
    }

    Vector3f Normalize(const Vector3f& v) {
        return FromGLM(glm::normalize(ToGLM(v)));
    }

    Vector3f Cross(const Vector3f& a, const Vector3f& b) {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    Matrix4f LookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up) {
        return FromGLM(glm::lookAt(ToGLM(eye), ToGLM(center), ToGLM(up)));
    }

    Matrix4f Perspective(float fov, float aspect, float near, float far) {
        return FromGLM(glm::perspective(fov, aspect, near, far));
    }
}
