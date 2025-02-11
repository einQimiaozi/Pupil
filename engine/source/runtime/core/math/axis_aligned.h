#pragma once

#include "runtime/core/math/vector3.h"
#include "runtime/tools/reflection/reflection.h"
#include <limits>

namespace Pupil {
    REFLECTION_TYPE(AxisAlignedBox)
    CLASS(AxisAlignedBox, Fields) {
        REFLECTION_BODY(AxisAlignedBox)
    public:
        AxisAlignedBox() {}
        AxisAlignedBox(const Vector3& center, const Vector3& half_extent);

        void merge(const Vector3& new_point);
        void update(const Vector3& center, const Vector3& half_extent);

        const Vector3& getCenter() const { return center; }
        const Vector3& getHalfExtent() const { return half_extent; }
        const Vector3& getMinCorner() const { return min_corner; }
        const Vector3& getMaxCorner() const { return max_corner; }

    private:
        Vector3 center {Vector3::ZERO};
        Vector3 half_extent {Vector3::ZERO};

        Vector3 min_corner {
            std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
        Vector3 max_corner {
            -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
    };
}