#pragma once

#include <optional>
#include <stdint.h>
#include <vector>

namespace Pupil
{
    // rhi class inteface, 这里只放数据类型, 也就是那些全局唯一(或者数组)类的对象
    class RHIBuffer { };
    class RHIBufferView { };
    class RHICommandBuffer { };
    class RHICommandPool { };
    class RHIDescriptorPool { };
    class RHIDescriptorSet { };
    class RHIDescriptorSetLayout { };
    class RHIDevice { };
    class RHIDeviceMemory { };
    class RHIFence { };
    class RHIFramebuffer { };
    class RHIImage { };
    class RHIImageView { };
    class RHIInstance { };
    class RHIQueue { };
    class RHIPipeline { };
    class RHIPipelineCache { };
    class RHIPipelineLayout { };
    class RHIRenderPass { };
    class RHISampler { };
    class RHISemaphore { };
    class RHIShader { };
    class RHIPhysicalDevice { };

    // rhi struct interface, 这里放的是创建上面class用到的对象及各种asset描述符，可以按照vulkan tutorial里涉及到的Vkxxx类型先放一些


}