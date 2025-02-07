#pragma once

#include <string>

#include "runtime/core/math/vector3.h"
#include "runtime/core/color/color.h"
#include "runtime/resource/type/data/camera_config.h"
#include "runtime/tools/reflection/reflection.h"

namespace Pupil {
    REFLECTION_TYPE(SkyBoxIrradianceMap)
    CLASS(SkyBoxIrradianceMap, Fields) {
        REFLECTION_BODY(SkyBoxIrradianceMap);
    public:
        std::string negative_x_map;
        std::string positive_x_map;
        std::string negative_y_map;
        std::string positive_y_map;
        std::string negative_z_map;
        std::string positive_z_map;
    };

    REFLECTION_TYPE(SkyBoxSpecularMap)
    CLASS(SkyBoxSpecularMap, Fields) {
        REFLECTION_BODY(SkyBoxSpecularMap);
    public:
        std::string negative_x_map;
        std::string positive_x_map;
        std::string negative_y_map;
        std::string positive_y_map;
        std::string negative_z_map;
        std::string positive_z_map;
    };

    REFLECTION_TYPE(DirectionalLight)
    CLASS(DirectionalLight, Fields) {
        REFLECTION_BODY(DirectionalLight);
    public:
        Vector3 direction;
        Color color;
    };

    REFLECTION_TYPE(GlobalRenderingRes)
    CLASS(GlobalRenderingRes, Fields) {
        REFLECTION_BODY(GlobalRenderingRes);
    public:
        bool                enable_fxaa {false};
        SkyBoxIrradianceMap skybox_irradiance_map;
        SkyBoxSpecularMap   skybox_specular_map;
        std::string         brdf_map;
        std::string         color_grading_map;

        Color            sky_color;
        Color            ambient_light;
        CameraConfig     camera_config;
        DirectionalLight directional_light;
    };
}