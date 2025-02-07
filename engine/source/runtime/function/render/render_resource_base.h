#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "runtime/function/render/render_swap_context.h"
#include "runtime/function/render/render_type.h"

namespace Pupil {
    class VulkanRHI;

    class RenderResourceBase {
    public:
        virtual ~RenderResourceBase() {}
        virtual void clear() = 0;
        virtual void uploadGlobalRenderResource(std::shared_ptr<VulkanRHI> rhi, LevelResourceDesc level_resource_desc) = 0;

        std::shared_ptr<TextureData> loadTextureHDR(std::string file, int desired_channels = 4);
        std::shared_ptr<TextureData> loadTexture(std::string file, bool is_srgb = false);
    };
}