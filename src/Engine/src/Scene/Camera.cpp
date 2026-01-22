//
// Created by ThewyRogue99 on 17.01.2026.
//

#include <Aether/Scene/Camera.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Math/Interop.h"

namespace Aether::Engine {
    void Camera::SetPosition(const Math::Vector3f& position) {
        m_Position = position;

        const auto rad = glm::radians(90.f);
    }

    void Camera::SetTarget(const Math::Vector3f& target) {
        m_Target = target;
    }

    void Camera::SetUp(const Math::Vector3f& up) {
        m_Up = up;
    }

    void Camera::SetPerspective(float fovRadians, float aspect, float nearPlane, float farPlane) {
        m_Fov = fovRadians;
        m_Aspect = aspect;
        m_NearPlane = nearPlane;
        m_FarPlane = farPlane;
    }

    Math::Matrix4f Camera::GetViewProjection() const {
        const glm::mat4 view = glm::lookAt(
            ToGLM(m_Position),
            ToGLM(m_Target),
            ToGLM(m_Up)
        );

        const glm::mat4 projection = glm::perspective(
            m_Fov,
            m_Aspect,
            m_NearPlane,
            m_FarPlane
        );

        return Math::FromGLM(projection * view);
    }
}
