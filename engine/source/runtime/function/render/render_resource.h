#pragma once

#include <memory>
#include <string>
#include <array>

#include <vulkan/vulkan_core.h>
#include <vk_mem_alloc.h>

#include "runtime/function/render/render_swap_context.h"
#include "runtime/function/render/render_resource_base.h"
#include "runtime/resource/type/global/global_rendering.h"

namespace Pupil {
    class VulkanRHI;

    struct IBLResource {
        VkImage _brdfLUT_texture_image;
        VkImageView _brdfLUT_texture_image_view;
        VkSampler _brdfLUT_texture_sampler;
        VmaAllocation _brdfLUT_texture_image_allocation;

        VkImage _irradiance_texture_image;
        VkImageView _irradiance_texture_image_view;
        VkSampler _irradiance_texture_sampler;
        VmaAllocation _irradiance_texture_image_allocation;

        VkImage _specular_texture_image;
        VkImageView _specular_texture_image_view;
        VkSampler _specular_texture_sampler;
        VmaAllocation _specular_texture_image_allocation;
    };

    struct ColorGradingResource {
        VkImage _color_grading_LUT_texture_image;
        VkImageView _color_grading_LUT_texture_image_view;
        VmaAllocation _color_grading_LUT_texture_image_allocation;
    };

    struct StorageBuffer {
        // limits
        uint32_t _min_uniform_buffer_offset_alignment{ 256 };
        uint32_t _min_storage_buffer_offset_alignment{ 256 };
        uint32_t _max_storage_buffer_range{ 1 << 27 };
        uint32_t _non_coherent_atom_size{ 256 };

        VkBuffer _global_upload_ringbuffer;
        VkDeviceMemory _global_upload_ringbuffer_memory;
        void* _global_upload_ringbuffer_memory_pointer;
        std::vector<uint32_t> _global_upload_ringbuffers_begin;
        std::vector<uint32_t> _global_upload_ringbuffers_end;
        std::vector<uint32_t> _global_upload_ringbuffers_size;

        VkBuffer _global_null_descriptor_storage_buffer;
        VkDeviceMemory _global_null_descriptor_storage_buffer_memory;

        // axis
        VkBuffer _axis_inefficient_storage_buffer;
        VkDeviceMemory _axis_inefficient_storage_buffer_memory;
        void* _axis_inefficient_storage_buffer_memory_pointer;
    };

    struct GlobalRenderResource {
        IBLResource          _ibl_resource;
        ColorGradingResource _color_grading_resource;
        StorageBuffer        _storage_buffer;
    };

    class RenderResource : public RenderResourceBase {
    public:
        void clear() override final;

        virtual void uploadGlobalRenderResource(std::shared_ptr<VulkanRHI> rhi, LevelResourceDesc level_resource_desc) override final;

        GlobalRenderResource global_render_resource;

    private:
        void createAndMapStorageBuffer(std::shared_ptr<VulkanRHI> rhi);
        void createIBLSamplers(std::shared_ptr<VulkanRHI> rhi);
        void createIBLTextures(std::shared_ptr<VulkanRHI> rhi, std::shared_ptr<BoxTextureData> irradiance_maps, std::shared_ptr<BoxTextureData> specular_maps);

        std::shared_ptr<BoxTextureData> loadBoxTexture(BoxMapBase boxMap);
    };
}