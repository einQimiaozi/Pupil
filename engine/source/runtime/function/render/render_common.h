#pragma once

#include <vk_mem_alloc.h>

#include "runtime/core/math/matrix4.h"
#include "vulkan/vulkan_core.h"

namespace Pupil {
    struct AxisStorageBufferObject {
        Matrix4x4 model_matrix = Matrix4x4::IDENTITY;
        uint32_t  selected_axis = 3;
    };

    struct VulkanMesh {
        bool enable_vertex_blending;
        uint32_t        mesh_vertex_count;
        VkBuffer        mesh_vertex_position_buffer;
        VmaAllocation   mesh_vertex_position_buffer_allocation;
        VkBuffer        mesh_vertex_varying_enable_blending_buffer;
        VmaAllocation   mesh_vertex_varying_enable_blending_buffer_allocation;
        VkBuffer        mesh_vertex_joint_binding_buffer;
        VmaAllocation   mesh_vertex_joint_binding_buffer_allocation;
        VkDescriptorSet mesh_vertex_blending_descriptor_set;
        VkBuffer        mesh_vertex_varying_buffer;
        VmaAllocation   mesh_vertex_varying_buffer_allocation;

        uint32_t        mesh_index_count;
        VkBuffer        mesh_index_buffer;
        VmaAllocation   mesh_index_buffer_allocation;
    };

    // material
    struct VulkanPBRMaterial {
        VkImage         base_color_texture_image;
        VkImageView     base_color_image_view;
        VmaAllocation   base_color_image_allocation;

        VkImage         metallic_roughness_texture_image;
        VkImageView     metallic_roughness_image_view;
        VmaAllocation   metallic_roughness_image_allocation;

        VkImage         normal_texture_image;
        VkImageView     normal_image_view;
        VmaAllocation   normal_image_allocation;

        VkImage         occlusion_texture_image;
        VkImageView     occlusion_image_view;
        VmaAllocation   occlusion_image_allocation;

        VkImage         emissive_texture_image;
        VkImageView     emissive_image_view;
        VmaAllocation   emissive_image_allocation;

        VkBuffer        material_uniform_buffer;
        VmaAllocation   material_uniform_buffer_allocation;

        VkDescriptorSet material_descriptor_set;
    };

    struct RenderMeshNode {
        const Matrix4x4*   model_matrix {nullptr};
        const Matrix4x4*   joint_matrices {nullptr};
        uint32_t           joint_count {0};
        VulkanMesh*        ref_mesh {nullptr};
        VulkanPBRMaterial* ref_material {nullptr};
        uint32_t           node_id;
        bool               enable_vertex_blending {false};
    };

    struct RenderAxisNode {
        Matrix4x4   model_matrix {Matrix4x4::IDENTITY};
        VulkanMesh* ref_mesh {nullptr};
        uint32_t    node_id;
        bool        enable_vertex_blending {false};
    };
}