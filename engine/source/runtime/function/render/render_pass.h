#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include "runtime/function/render/render_common.h"
#include "runtime/function/render/render_pass_base.h"
#include "runtime/function/render/render_resource.h"

namespace Pupil {
    class VulkanRHI;
    enum {
        _main_camera_pass_gbuffer_a                     = 0,
        _main_camera_pass_gbuffer_b                     = 1,
        _main_camera_pass_gbuffer_c                     = 2,
        _main_camera_pass_backup_buffer_odd             = 3,
        _main_camera_pass_backup_buffer_even            = 4,
        _main_camera_pass_post_process_buffer_odd       = 5,
        _main_camera_pass_post_process_buffer_even      = 6,
        _main_camera_pass_depth                         = 7,
        _main_camera_pass_swap_chain_image              = 8,
        _main_camera_pass_custom_attachment_count       = 5,
        _main_camera_pass_post_process_attachment_count = 2,
        _main_camera_pass_attachment_count              = 9,
    };

    enum {
        _main_camera_subpass_basepass = 0,
        _main_camera_subpass_deferred_lighting,
        _main_camera_subpass_forward_lighting,
        _main_camera_subpass_tone_mapping,
        _main_camera_subpass_color_grading,
        _main_camera_subpass_fxaa,
        _main_camera_subpass_ui,
        _main_camera_subpass_combine_ui,
        _main_camera_subpass_count
    };

    // 可见物
    struct VisiableNodes {
        std::vector<RenderMeshNode>* p_directional_light_visible_mesh_nodes {nullptr};
        std::vector<RenderMeshNode>* p_point_lights_visible_mesh_nodes {nullptr};
        std::vector<RenderMeshNode>* p_main_camera_visible_mesh_nodes {nullptr};
        RenderAxisNode*              p_axis_node {nullptr};
    };

    class RenderPass : public RenderPassBase {
    public:
        // buffer用到的附着
        struct FrameBufferAttachment {
            VkImage         image;
            VkDeviceMemory  mem;
            VkImageView     view;
            VkFormat        format;
        };
        // 用附着造个buffer吧!!!
        struct Framebuffer {
            int           width;
            int           height;
            VkFramebuffer framebuffer;
            VkRenderPass  render_pass;

            std::vector<FrameBufferAttachment> attachments;
        };
        
        // shader描述符结构
        struct Descriptor {
            VkDescriptorSetLayout layout;
            VkDescriptorSet       descriptor_set;
        };
        
        // 渲染管线
        struct RenderPipelineBase {
            VkPipelineLayout layout;
            VkPipeline       pipeline;
        };

        // 全局资源
        GlobalRenderResource* global_render_resource {nullptr};

        std::vector<Descriptor>         descriptor_infos;
        std::vector<RenderPipelineBase> render_pipelines;
        Framebuffer                     framebuffer;

        void initialize(const RenderPassInitInfo* init_info) override;
        void postInitialize() override;

        virtual void draw();

        virtual VkRenderPass                        getRenderPass() const;
        virtual std::vector<VkImageView>            getFramebufferImageViews() const;
        virtual std::vector<VkDescriptorSetLayout>  getDescriptorSetLayouts() const;

        static VisiableNodes visiable_nodes;
    };
}