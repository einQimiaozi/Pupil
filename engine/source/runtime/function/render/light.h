#pragma once

#include <vector>

#include "runtime/core/math/vector3.h"

#include "runtime/function/render/render_type.h"

namespace Pupil {
    struct PointLight {
        Vector3 position;
        // radiant flux in W
        Vector3 flux;

        // 计算光照半径, 半径外的物体不相应光照(性能优化, 非真实物理反馈), 参数可调节(flux)，用于支持延迟渲染的空间划分(light culling)
        float calculateRadius() const {
            // radius = where attenuation would lead to an intensity of 1W/m^2
            const float INTENSITY_CUTOFF    = 1.0f;
            const float ATTENTUATION_CUTOFF = 0.05f;
            Vector3     intensity           = flux / (4.0f * Math_PI);
            float       maxIntensity        = Vector3::getMaxElement(intensity);
            float       attenuation         = Math::max(INTENSITY_CUTOFF, ATTENTUATION_CUTOFF * maxIntensity) / maxIntensity;
            return 1.0f / sqrtf(attenuation);
        }
    };

    // 环境光
    struct AmbientLight {
        Vector3 irradiance;
    };

    // 平行光
    struct PDirectionalLight {
        Vector3 direction;
        Vector3 color;
    };

    // 单个tile上可见光列表
    struct LightList {
        struct PointLightVertex {
            Vector3 position;
            float   padding;
            Vector3 intensity;
            float   radius;
        };
    };

    class PointLightList : public LightList {
    public:
        void init() {}
        void shutdown() {}

        // upload changes to GPU
        void update() {}

        std::vector<PointLight> lights;
        std::shared_ptr<BufferData> buffer;
    };
}