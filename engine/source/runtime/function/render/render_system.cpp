#include "render_system.h"
#include "runtime/resource/config_manager/config_manager.h"
#include "runtime/resource/asset_manager/asset_manager.h"
#include "runtime/resource/type/global/global_rendering.h"
#include "runtime/function/render/render_resource.h"
#include "runtime/core/macro.h"
#include "runtime/platform/rhi/vulkan/vulkan_rhi.h"
#include "runtime/platform/rhi/vulkan/vulkan_struct.h"

namespace Pupil {
    void RenderSystem::initialize(RenderInterface interface) {
        std::shared_ptr<ConfigManager> config_manager = runtime_global_context.config_manager;
        assert(config_manager);
        std::shared_ptr<AssetManager> asset_manager = runtime_global_context.asset_manager;
        assert(asset_manager);

        rhi = std::make_shared<VulkanRHI>();
        VulkanConfig v_config;
        rhi->initiative(interface, v_config);

        // global rendering resource
        GlobalRenderingRes global_rendering_res;
        const std::string& global_rendering_res_url = config_manager->global_rendering_res_path;
        // 获取所有我们需要在初始化阶段创建的资源 天空盒 brdf预计算纹理 color grading纹理
        asset_manager->loadAsset(global_rendering_res_url, global_rendering_res);

        // upload ibl, color grading textures
        // 这里的level_resource_desc感觉就是一个中间临时对象
        LevelResourceDesc level_resource_desc;
        level_resource_desc.ibl_resource_desc.skybox_irradiance_map       = global_rendering_res.skybox_irradiance_map;
        level_resource_desc.ibl_resource_desc.skybox_specular_map         = global_rendering_res.skybox_specular_map;
        level_resource_desc.ibl_resource_desc.brdf_map                    = global_rendering_res.brdf_map;
        level_resource_desc.color_grading_resource_desc.color_grading_map = global_rendering_res.color_grading_map;

        // 把天空盒 brdf预计算纹理 color grading纹理创建好，数据写入到m_global_render_resource的成员中，后续可以直接用
        render_resource = std::make_shared<RenderResource>();
        render_resource->uploadGlobalRenderResource(rhi, level_resource_desc);
    }
    
    void RenderSystem::tick(float delta_time) {
        // todo: process swap data between logic and render contexts

    }
}