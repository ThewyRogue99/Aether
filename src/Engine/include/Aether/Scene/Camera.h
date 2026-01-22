//
// Created by ThewyRogue99 on 17.01.2026.
//

#pragma once

#include <Aether/Math/Matrix.h>

namespace Aether::Engine {
    class Camera {
    public:
        void SetPosition(const Math::Vector3f& position);
        void SetTarget(const Math::Vector3f& target);
        void SetUp(const Math::Vector3f& up);

        void SetPerspective(float fovRadians, float aspect, float nearPlane, float farPlane);

        [[nodiscard]] Math::Matrix4f GetViewProjection() const;

    private:
        Math::Vector3f m_Position { 0.0f, 0.0f, 3.0f };
        Math::Vector3f m_Target { 0.0f, 0.0f, 0.0f };
        Math::Vector3f m_Up { 0.0f, 1.0f, 0.0f };

        float m_Fov = 60.0f;
        float m_Aspect = 1.0f;
        float m_NearPlane = 0.1f;
        float m_FarPlane = 100.0f;
    };
}
