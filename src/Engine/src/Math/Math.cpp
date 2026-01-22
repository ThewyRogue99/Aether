//
// Created by ThewyRogue99 on 17.01.2026.
//

#include <Aether/Math/Math.h>

#include <glm/trigonometric.hpp>
#include <glm/mat4x4.hpp>

#include "glm/ext/matrix_transform.hpp"

namespace Aether::Math {
    float Radians(float degrees) {
        return glm::radians(degrees);
    }

    float Degrees(float radians) {
        return glm::degrees(radians);
    }

    Matrix4f Translate(const Matrix4f& m, const Vector3f& v) {
        Matrix4f result = m;

        result[3][0] = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0];
        result[3][1] = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1];
        result[3][2] = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2];
        result[3][3] = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3];

        return result;
    }
}
