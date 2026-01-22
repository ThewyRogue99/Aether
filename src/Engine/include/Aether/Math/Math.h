//
// Created by ThewyRogue99 on 17.01.2026.
//

#pragma once

#include <Aether/Math/Matrix.h>

namespace Aether::Math {
    float Radians(float degrees);
    float Degrees(float radians);

    Matrix4f Translate(const Matrix4f& m, const Vector3f& v);
}
