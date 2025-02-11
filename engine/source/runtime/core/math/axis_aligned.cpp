#include "runtime/core/math/axis_aligned.h"

namespace Pupil {
    AxisAlignedBox::AxisAlignedBox(const Vector3& center, const Vector3& half_extent) { update(center, half_extent); }
    void AxisAlignedBox::merge(const Vector3& new_point) {
        min_corner.makeFloor(new_point);
        max_corner.makeCeil(new_point);

        center      = 0.5f * (min_corner + max_corner);
        half_extent = center - min_corner;
    }

    void AxisAlignedBox::update(const Vector3& center, const Vector3& half_extent) {
        this->center      = center;
        this->half_extent = half_extent;
        min_corner  = center - half_extent;
        max_corner  = center + half_extent;
    }
}