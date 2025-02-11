#pragma once

#include "runtime/platform/rhi/vulkan/vulkan_rhi.h"

namespace Pupil {
    class VulkanRHI;
    class RenderResourceBase;
    class WindowUI;

    struct RenderPassInitInfo {};

    // render pass就是rhi的一段管线，包含rhi和资源即可
    struct RenderPassCommonInfo {
        std::shared_ptr<VulkanRHI>          rhi;
        std::shared_ptr<RenderResourceBase> render_resource;
    };

    class RenderPassBase {
    public:
        virtual void initialize(const RenderPassInitInfo* init_info) = 0;
        virtual void postInitialize();
        virtual void setCommonInfo(RenderPassCommonInfo common_info);
        virtual void preparePassData(std::shared_ptr<RenderResourceBase> render_resource);
        virtual void initializeUIRenderBackend(WindowUI* window_ui);

    protected:
        std::shared_ptr<VulkanRHI>          rhi;
        std::shared_ptr<RenderResourceBase> render_resource;
    };
}