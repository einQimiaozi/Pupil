#include <algorithm>
#include <filesystem>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <vulkan/vulkan_core.h>

#include "runtime/function/render/render_resource_base.h"
#include "runtime/resource/config_manager/config_manager.h"
#include "runtime/function/global/global_context.h"

namespace Pupil {
    std::shared_ptr<TextureData> RenderResourceBase::loadTextureHDR(std::string file, int desired_channels) {
        std::shared_ptr<TextureData> texture = std::make_shared<TextureData>();

        int iw, ih, n;
        texture->pixels = stbi_loadf(std::filesystem::absolute(runtime_global_context.config_manager->root_path / file).generic_string().c_str(), &iw, &ih, &n, desired_channels);
        if (!texture->pixels) {
            return nullptr;
        }

        texture->width  = iw;
        texture->height = ih;
        switch (desired_channels) {
            case 2:
                texture->format = VkFormat::VK_FORMAT_R32G32_SFLOAT;
                break;
            case 4:
                texture->format = VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
                break;
            default:
                throw std::runtime_error("unsupported channels number");
                break;
        }
        texture->depth        = 1;
        texture->array_layers = 1;
        texture->mip_levels   = 1;
        texture->type         = VkImageType::VK_IMAGE_TYPE_2D;

        return texture;
    }

    std::shared_ptr<TextureData> RenderResourceBase::loadTexture(std::string file, bool is_srgb) {
        std::shared_ptr<TextureData> texture = std::make_shared<TextureData>();

        int iw, ih, n;
        texture->pixels = stbi_load(std::filesystem::absolute(runtime_global_context.config_manager->root_path / file).generic_string().c_str(), &iw, &ih, &n, 4);

        if (!texture->pixels) {
            return nullptr;
        }

        texture->width        = iw;
        texture->height       = ih;
        texture->format       = (is_srgb) ? VkFormat::VK_FORMAT_R8G8B8A8_SRGB :
                                              VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
        texture->depth        = 1;
        texture->array_layers = 1;
        texture->mip_levels   = 1;
        texture->type         = VkImageType::VK_IMAGE_TYPE_2D;

        return texture;
    }
}