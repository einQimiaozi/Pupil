#pragma once

#include "runtime/core/math/vector2.h"
#include "runtime/core/math/vector3.h"
#include "runtime/tools/reflection/reflection.h"

namespace Pupil {
    REFLECTION_TYPE(CameraPose)
    CLASS(CameraPose, Fields) {
        REFLECTION_BODY(CameraPose);
    public:
        Vector3 position;
        Vector3 target;
        Vector3 up;
    };

    REFLECTION_TYPE(CameraConfig)
    CLASS(CameraConfig, Fields) {
        REFLECTION_BODY(CameraConfig);
    public:
        CameraPose pose;
        Vector2    aspect;
        float      z_far;
        float      z_near;
    };
}