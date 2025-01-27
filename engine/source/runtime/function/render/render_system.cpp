#include "render_system.h"
#include "platform/rhi/vulkan/vulkan_rhi.h"
#include "platform/rhi/vulkan/vulkan_struct.h"

namespace Pupil {
    void RenderSystem::initialize(RenderInterface interface) {
        rhi = std::make_shared<VulkanRHI>();
        VulkanConfig v_config;
        rhi->initiative(interface, v_config);
    }
}