#include <stdexcept>

#include "runtime/function/render/render_resource.h"
#include "runtime/function/render/render_resource_base.h"
#include "runtime/function/render/render_type.h"
#include "runtime/function/render/render_mesh.h"

#include "runtime/function/render/render_common.h"

#include "runtime/platform/rhi/vulkan/vulkan_rhi.h"
#include "runtime/platform/rhi/vulkan/vulkan_utils.h"

#include "runtime/core/macro.h"

namespace Pupil {
    void RenderResource::clear() {}

    void RenderResource::
    uploadGlobalRenderResource(std::shared_ptr<VulkanRHI> rhi, LevelResourceDesc level_resource_desc) {
        // create and map global storage buffer
        // 把渲染用的一些buffer通过rhi映射到cpu端，后续可以用global_render_resource._storage_buffer这个成员在cpu端操作内存
        createAndMapStorageBuffer(rhi);

        // 这部分就是单纯的读贴图，转换为TextureData格式
        // sky box irradiance
        SkyBoxIrradianceMap skybox_irradiance_map        = level_resource_desc.ibl_resource_desc.skybox_irradiance_map;
        std::shared_ptr<TextureData> irradiace_pos_x_map = loadTextureHDR(skybox_irradiance_map.positive_x_map);
        std::shared_ptr<TextureData> irradiace_neg_x_map = loadTextureHDR(skybox_irradiance_map.negative_x_map);
        std::shared_ptr<TextureData> irradiace_pos_y_map = loadTextureHDR(skybox_irradiance_map.positive_y_map);
        std::shared_ptr<TextureData> irradiace_neg_y_map = loadTextureHDR(skybox_irradiance_map.negative_y_map);
        std::shared_ptr<TextureData> irradiace_pos_z_map = loadTextureHDR(skybox_irradiance_map.positive_z_map);
        std::shared_ptr<TextureData> irradiace_neg_z_map = loadTextureHDR(skybox_irradiance_map.negative_z_map);

        // sky box specular
        SkyBoxSpecularMap            skybox_specular_map = level_resource_desc.ibl_resource_desc.skybox_specular_map;
        std::shared_ptr<TextureData> specular_pos_x_map  = loadTextureHDR(skybox_specular_map.positive_x_map);
        std::shared_ptr<TextureData> specular_neg_x_map  = loadTextureHDR(skybox_specular_map.negative_x_map);
        std::shared_ptr<TextureData> specular_pos_y_map  = loadTextureHDR(skybox_specular_map.positive_y_map);
        std::shared_ptr<TextureData> specular_neg_y_map  = loadTextureHDR(skybox_specular_map.negative_y_map);
        std::shared_ptr<TextureData> specular_pos_z_map  = loadTextureHDR(skybox_specular_map.positive_z_map);
        std::shared_ptr<TextureData> specular_neg_z_map  = loadTextureHDR(skybox_specular_map.negative_z_map);

        // brdf
        std::shared_ptr<TextureData> brdf_map = loadTextureHDR(level_resource_desc.ibl_resource_desc.brdf_map);

        // create IBL samplers
        createIBLSamplers(rhi);

        // create IBL textures, take care of the texture order
        std::array<std::shared_ptr<TextureData>, 6> irradiance_maps = {irradiace_pos_x_map,
                                                                       irradiace_neg_x_map,
                                                                       irradiace_pos_z_map,
                                                                       irradiace_neg_z_map,
                                                                       irradiace_pos_y_map,
                                                                       irradiace_neg_y_map};
        std::array<std::shared_ptr<TextureData>, 6> specular_maps   = {specular_pos_x_map,
                                                                     specular_neg_x_map,
                                                                     specular_pos_z_map,
                                                                     specular_neg_z_map,
                                                                     specular_pos_y_map,
                                                                     specular_neg_y_map};

        createIBLTextures(rhi, irradiance_maps, specular_maps);

        // 创建 _brdfLUT_texture_image 纹理三件套
        rhi->createGlobalImage(
            global_render_resource._ibl_resource._brdfLUT_texture_image,
            global_render_resource._ibl_resource._brdfLUT_texture_image_view,
            global_render_resource._ibl_resource._brdfLUT_texture_image_allocation,
            brdf_map->width,
            brdf_map->height,
            brdf_map->pixels,
            brdf_map->format
        );

        // 获取color grading纹理，保存在color_grading_map中
        std::shared_ptr<TextureData> color_grading_map =
            loadTexture(level_resource_desc.color_grading_resource_desc.color_grading_map);

        // create color grading texture
        rhi->createGlobalImage(
            global_render_resource._color_grading_resource._color_grading_LUT_texture_image,
            global_render_resource._color_grading_resource._color_grading_LUT_texture_image_view,
            global_render_resource._color_grading_resource._color_grading_LUT_texture_image_allocation,
            color_grading_map->width,
            color_grading_map->height,
            color_grading_map->pixels,
            color_grading_map->format,
            1
        );
        // todo: destory sampler image imageView
    }

    void RenderResource::createAndMapStorageBuffer(std::shared_ptr<VulkanRHI> rhi) {
        StorageBuffer& _storage_buffer = global_render_resource._storage_buffer;
        uint32_t       frames_in_flight = rhi->maxFrameFlight;

        VkPhysicalDeviceProperties properties = rhi->getPhysicalDeviceProperties();

        _storage_buffer._min_uniform_buffer_offset_alignment = static_cast<uint32_t>(properties.limits.minUniformBufferOffsetAlignment);
        _storage_buffer._min_storage_buffer_offset_alignment = static_cast<uint32_t>(properties.limits.minStorageBufferOffsetAlignment);
        _storage_buffer._max_storage_buffer_range = properties.limits.maxStorageBufferRange;
        _storage_buffer._non_coherent_atom_size = properties.limits.nonCoherentAtomSize;

        // _global_upload_ringbuffer保存的是全局渲染资源的vulkan buffer，根据并行数分为三块
        uint32_t global_storage_buffer_size = 1024 * 1024 * 128;
        rhi->createBuffer(global_storage_buffer_size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                          _storage_buffer._global_upload_ringbuffer,
                          _storage_buffer._global_upload_ringbuffer_memory);

        _storage_buffer._global_upload_ringbuffers_begin.resize(frames_in_flight);
        _storage_buffer._global_upload_ringbuffers_end.resize(frames_in_flight);
        _storage_buffer._global_upload_ringbuffers_size.resize(frames_in_flight);
        for (uint32_t i = 0; i < frames_in_flight; ++i) {
            _storage_buffer._global_upload_ringbuffers_begin[i] = (global_storage_buffer_size * i) / frames_in_flight;
            _storage_buffer._global_upload_ringbuffers_size[i] = (global_storage_buffer_size * (i + 1)) / frames_in_flight - (global_storage_buffer_size * i) / frames_in_flight;
        }

        rhi->createBuffer(sizeof(AxisStorageBufferObject),
                          VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                          _storage_buffer._axis_inefficient_storage_buffer,
                          _storage_buffer._axis_inefficient_storage_buffer_memory);

        rhi->createBuffer(64,
                          VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                          0,
                          _storage_buffer._global_null_descriptor_storage_buffer,
                          _storage_buffer._global_null_descriptor_storage_buffer_memory);

        // TODO: Unmap when program terminates
        rhi->mapMemory(_storage_buffer._global_upload_ringbuffer_memory,
                       0,
                       VK_WHOLE_SIZE,
                       0,
                       &_storage_buffer._global_upload_ringbuffer_memory_pointer);

        rhi->mapMemory(_storage_buffer._axis_inefficient_storage_buffer_memory,
                       0,
                       VK_WHOLE_SIZE,
                       0,
                       &_storage_buffer._axis_inefficient_storage_buffer_memory_pointer);
        // 需要MeshVertex::VulkanMeshVertexJointBinding的内存块大于64才可以，不然后面分配内存可能不够
        static_assert(64 >= sizeof(MeshVertex::VulkanMeshVertexJointBinding), "");
    }

    void RenderResource::createIBLSamplers(std::shared_ptr<VulkanRHI> rhi) {
        VkPhysicalDeviceProperties physical_device_properties = rhi->getPhysicalDeviceProperties();

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.anisotropyEnable = VK_TRUE;   
        samplerInfo.maxAnisotropy = physical_device_properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.maxLod = 0.0f;

        // 给IBL的brdf材质创建一个采样器
        if (global_render_resource._ibl_resource._brdfLUT_texture_sampler != VK_NULL_HANDLE) {
            rhi->destroySampler(global_render_resource._ibl_resource._brdfLUT_texture_sampler);
        }

        if (rhi->createSampler(&samplerInfo, global_render_resource._ibl_resource._brdfLUT_texture_sampler) != VK_SUCCESS) {
            throw std::runtime_error("vk create sampler failed");
        }

        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 8.0f;
        samplerInfo.mipLodBias = 0.0f;

        // 给IBL的irradiance纹理创建一个采样器
        if (global_render_resource._ibl_resource._irradiance_texture_sampler != VK_NULL_HANDLE) {
            rhi->destroySampler(global_render_resource._ibl_resource._irradiance_texture_sampler);
        }

        if (rhi->createSampler(&samplerInfo, global_render_resource._ibl_resource._irradiance_texture_sampler) != VK_SUCCESS) {
            throw std::runtime_error("vk create sampler failed");
        }

        // 给IBL的specular纹理创建一个采样器
        if (global_render_resource._ibl_resource._specular_texture_sampler != VK_NULL_HANDLE) {
            rhi->destroySampler(global_render_resource._ibl_resource._specular_texture_sampler);
        }

        if (rhi->createSampler(&samplerInfo, global_render_resource._ibl_resource._specular_texture_sampler) != VK_SUCCESS) {
            throw std::runtime_error("vk create sampler failed");
        }
    }

    void RenderResource::createIBLTextures(std::shared_ptr<VulkanRHI> rhi, 
        std::array<std::shared_ptr<TextureData>, 6> irradiance_maps, 
        std::array<std::shared_ptr<TextureData>, 6> specular_maps
    ) {
        // 计算mipmap等级
        uint32_t irradiance_cubemap_miplevels = static_cast<uint32_t>(std::floor(log2(std::max(irradiance_maps[0]->width, irradiance_maps[0]->height)))) + 1;
        // 创建vulkan可用的_irradiance_texture_image贴图三件套
        rhi->createCubeMap(
            global_render_resource._ibl_resource._irradiance_texture_image,
            global_render_resource._ibl_resource._irradiance_texture_image_view,
            global_render_resource._ibl_resource._irradiance_texture_image_allocation,
            irradiance_maps[0]->width,
            irradiance_maps[0]->height,
            { 
                irradiance_maps[0]->pixels,
                irradiance_maps[1]->pixels,
                irradiance_maps[2]->pixels,
                irradiance_maps[3]->pixels,
                irradiance_maps[4]->pixels,
                irradiance_maps[5]->pixels 
            },
            irradiance_maps[0]->format,
            irradiance_cubemap_miplevels
        );

        uint32_t specular_cubemap_miplevels = static_cast<uint32_t>(std::floor(log2(std::max(specular_maps[0]->width, specular_maps[0]->height)))) + 1;
        // 创建vulkan可用的_specular_texture_image贴图三件套
        rhi->createCubeMap(
            global_render_resource._ibl_resource._specular_texture_image,
            global_render_resource._ibl_resource._specular_texture_image_view,
            global_render_resource._ibl_resource._specular_texture_image_allocation,
            specular_maps[0]->width,
            specular_maps[0]->height,
            { 
                specular_maps[0]->pixels,
                specular_maps[1]->pixels,
                specular_maps[2]->pixels,
                specular_maps[3]->pixels,
                specular_maps[4]->pixels,
                specular_maps[5]->pixels 
            },
            specular_maps[0]->format,
            specular_cubemap_miplevels
        );
    }
}