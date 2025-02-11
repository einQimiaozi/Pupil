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

    class BoxTextureData {
    public:
        std::shared_ptr<TextureData> negative_x_map;
        std::shared_ptr<TextureData> positive_x_map;
        std::shared_ptr<TextureData> negative_y_map;
        std::shared_ptr<TextureData> positive_y_map;
        std::shared_ptr<TextureData> negative_z_map;
        std::shared_ptr<TextureData> positive_z_map;

        BoxTextureData(
            std::shared_ptr<TextureData> positive_x,
            std::shared_ptr<TextureData> negative_x,
            std::shared_ptr<TextureData> positive_y,
            std::shared_ptr<TextureData> negative_y,
            std::shared_ptr<TextureData> positive_z,
            std::shared_ptr<TextureData> negative_z
        ) {
            negative_x_map = negative_x;
            positive_x_map = positive_x;
            negative_y_map = negative_y;
            positive_y_map = positive_y;
            negative_z_map = negative_z;
            positive_z_map = positive_z;
        }
    };

    // 通用buffer
    class BufferData {
    public:
        size_t size {0};
        void*  data {nullptr};

        BufferData() = delete;
        BufferData(size_t size) {
            size = size;
            data = malloc(size);
        }
        ~BufferData() {
            if (data) {
                free(data);
            }
        }
        bool isValid() const { return data != nullptr; }
    };
}
#endif