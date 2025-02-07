#include "runtime/core/math/vector4.h"

namespace Pupil {
    namespace MeshVertex {
        // 4个顶点, 绑定四个关节权重
        struct VulkanMeshVertexJointBinding {
            int indices[4];
            Vector4  weights;
        };
    }
}