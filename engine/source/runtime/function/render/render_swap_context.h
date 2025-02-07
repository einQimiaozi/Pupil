#ifndef __RENDER_SWAP_CONTEXT_H__
#define __RENDER_SWAP_CONTEXT_H__

#pragma once

#include <string>

#include "runtime/resource/type/global/global_rendering.h"

namespace Pupil {    
    struct LevelIBLResourceDesc {
        SkyBoxIrradianceMap skybox_irradiance_map;
        SkyBoxSpecularMap   skybox_specular_map;
        std::string         brdf_map;
    };

    struct LevelColorGradingResourceDesc {
        std::string color_grading_map;
    };

    struct LevelResourceDesc {
        LevelIBLResourceDesc          ibl_resource_desc;
        LevelColorGradingResourceDesc color_grading_resource_desc;
    };
}
#endif