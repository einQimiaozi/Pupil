#ifndef __RENDER_TYPE_H__
#define __RENDER_TYPE_H__

#pragma once

#include <stdint.h>
#include <cstdint>
#include <memory>

#include <vulkan/vulkan_core.h>

namespace Pupil {
    class TextureData {
    public:
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t depth = 0;
        uint32_t mip_levels = 0;
        uint32_t array_layers = 0;
        void*    pixels {nullptr};

        VkFormat format = VK_FORMAT_MAX_ENUM;
        VkImageType type = VkImageType::VK_IMAGE_TYPE_MAX_ENUM;

        TextureData() = default;
        ~TextureData() {
            if (pixels) {
                free(pixels);
            }
        }
        bool isValid() const { return pixels != nullptr; }
    };
}
#endif