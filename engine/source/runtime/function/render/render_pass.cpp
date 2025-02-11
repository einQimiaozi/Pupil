#include "vulkan/vulkan_core.h"

#include "runtime/core/macro.h"

#include "runtime/function/render/render_pass.h"
#include "runtime/function/render/render_resource.h"
#include "runtime/platform/rhi/vulkan/vulkan_rhi.h"

// 全局可见物
Pupil::VisiableNodes Pupil::RenderPass::visiable_nodes;

namespace Pupil {
    void RenderPass::initialize(const RenderPassInitInfo* init_info) {
        global_render_resource = &(std::static_pointer_cast<RenderResource>(render_resource)->global_render_resource);
    }

    void RenderPass::draw() {}

    void RenderPass::postInitialize() {}

    VkRenderPass RenderPass::getRenderPass() const { return framebuffer.render_pass; }

    std::vector<VkImageView> RenderPass::getFramebufferImageViews() const {
        std::vector<VkImageView> image_views;
        for (auto& attach : framebuffer.attachments) {
            image_views.push_back(attach.view);
        }
        return image_views;
    }

    std::vector<VkDescriptorSetLayout> RenderPass::getDescriptorSetLayouts() const {
        std::vector<VkDescriptorSetLayout> layouts;
        for (auto& desc : descriptor_infos) {
            layouts.push_back(desc.layout);
        }
        return layouts;
    }
}