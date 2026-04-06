//
// Created by ThewyRogue99 on 17.01.2026.
//

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

#include <Aether/Math/Vector.h>
#include <Aether/Math/Matrix.h>

namespace Aether::Math {
    // ============================
    // Vector → GLM
    // ============================

    inline glm::vec2 ToGLM(const Vector2f& v) {
        return {v.x, v.y};
    }

    inline glm::vec3 ToGLM(const Vector3f& v) {
        return {v.x, v.y, v.z};
    }

    inline glm::vec4 ToGLM(const Vector4f& v) {
        return {v.x, v.y, v.z, v.w};
    }

    // ============================
    // GLM → Vector
    // ============================

    inline Vector2f FromGLM(const glm::vec2& v) {
        return { v.x, v.y };
    }

    inline Vector3f FromGLM(const glm::vec3& v) {
        return { v.x, v.y, v.z };
    }

    inline Vector4f FromGLM(const glm::vec4& v) {
        return { v.x, v.y, v.z, v.w };
    }

    // ============================
    // Matrix → GLM
    // ============================

    inline glm::mat4 ToGLM(const Matrix4f& m) {
        glm::mat4 out{ 1.0f };

        for (int col = 0; col < 4; ++col) {
            for (int row = 0; row < 4; ++row) {
                out[col][row] = m[row][col];
            }
        }

        return out;
    }

    // ============================
    // GLM → Matrix
    // ============================

    inline Matrix4f FromGLM(const glm::mat4& m) {
        Matrix4f out{};

        for (int col = 0; col < 4; ++col) {
            for (int row = 0; row < 4; ++row) {
                out[row][col] = m[col][row];
            }
        }

        return out;
    }
}
