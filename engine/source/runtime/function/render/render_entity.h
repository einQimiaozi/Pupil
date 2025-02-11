#ifndef __RENDER_ENTITY_H__
#define __RENDER_ENTITY_H__

#pragma once

#include <cstdint>
#include <vector>

#include "runtime/core/math/axis_aligned.h"
#include "runtime/core/math/matrix4.h"

namespace Pupil {
    class RenderEntity {
    public:
        uint32_t    instance_id {0};
        Matrix4x4   model_matrix {Matrix4x4::IDENTITY};

        // mesh
        size_t                 mesh_asset_id {0};
        bool                   enable_vertex_blending {false};
        std::vector<Matrix4x4> joint_matrices;
        AxisAlignedBox         bounding_box;

        // material
        size_t      material_asset_id {0};
        bool        blend {false};
        bool        double_sided {false};
        Vector4     base_color_factor {1.0f, 1.0f, 1.0f, 1.0f};
        float       metallic_factor {1.0f};
        float       roughness_factor {1.0f};
        float       normal_scale {1.0f};
        float       occlusion_strength {1.0f};
        Vector3     emissive_factor {0.0f, 0.0f, 0.0f};
    };
}
#endif