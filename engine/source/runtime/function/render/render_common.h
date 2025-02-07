#pragma once

#include "runtime/core/math/matrix4.h"

namespace Pupil {
    struct AxisStorageBufferObject {
        Matrix4x4 model_matrix  = Matrix4x4::IDENTITY;
        uint32_t  selected_axis = 3;
    };
}