#include "vulkan_rhi.h"
#include "runtime/core/macro.h"
#include "runtime/platform/rhi/vulkan/vulkan_utils.h"
#include "runtime/function/render/window_system.h"

namespace Pupil {
	VkResult VulkanRHI::createInstance() {
		std::string version = getVulkanVersionStr();
		LOG_INFO("vulkan version: " + version);

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "PupilRuntime";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Pupil-Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;
		appInfo.pNext = nullptr;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// 获取必要拓展
		std::vector<const char*> extensions = getRequiredExtensions(config.enableValidation);
		if (config.enableValidation && !checkExtensionSupport(extensionsName)) {
			LOG_ERROR("extenstions requested, but not available!");
			return VK_ERROR_INITIALIZATION_FAILED;
		}
		if (config.enableValidation && !checkValidationLayerSupport(validationLayers)) {
			LOG_ERROR("layers requested, but not available!");
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		if (config.enableValidation) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();
		extensionsName = extensions;

		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
		if (result != VK_SUCCESS) {
			return result;
		}

		if (!config.enableValidation) {
			return VK_SUCCESS;
		}

		VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = {};
		debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugMessengerCreateInfo.pfnUserCallback = debugCallback;
		debugMessengerCreateInfo.pUserData = nullptr;
		// 使用CreateDebugUtilsMessengerEXT创建debug messenger
		if ((result = CreateDebugUtilsMessengerEXT(instance, &debugMessengerCreateInfo, nullptr, &callback)) != VK_SUCCESS) {
			return result;
		}
		return VK_SUCCESS;
	}

	VkResult VulkanRHI::CreateDebugUtilsMessengerEXT(VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pCallback
	) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	VkResult VulkanRHI::createWindowSurface() {
		return glfwCreateWindowSurface(instance, window, nullptr, &surface);
	}

	VkResult VulkanRHI::pickPhysicalDevice() {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0) {
			LOG_ERROR("failed to find GPUs with Vulkan support!");
			return VK_ERROR_INITIALIZATION_FAILED;
		}
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
		int score = -0x3f3f3f3f;
		for (auto device : devices) {
			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			int deviceScore = 0;
			deviceScore += (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) * 1000;
			deviceScore += deviceProperties.limits.maxImageDimension2D;
			deviceScore += std::min(deviceProperties.limits.framebufferColorSampleCounts, deviceProperties.limits.framebufferDepthSampleCounts) * 100;
			deviceScore *= deviceFeatures.geometryShader;
			physicalDevice = device;

			if (isDeviceSuitable(physicalDevice, deviceExtensions, surface) && deviceScore > 0 && score < deviceScore) {
				score = deviceScore;
				break;
			}
		}
		if (physicalDevice == VK_NULL_HANDLE) {
			LOG_ERROR("failed to find a suitable GPU!");
			return VK_ERROR_INITIALIZATION_FAILED;
		}
		return VK_SUCCESS;
	}

	VkResult VulkanRHI::createLogicalDevice() {
		VkResult result = VK_SUCCESS;
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };
		for (int queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
			queueCreateInfo.queueCount = 1;
			float queuePriorities = 1.0f;
			queueCreateInfo.pQueuePriorities = &queuePriorities;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		// 配置硬件特性, 后续新增硬件特性在这里写
		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		if (config.enableRateShading) {
			deviceFeatures.sampleRateShading = VK_TRUE;
		}
		deviceFeatures.fragmentStoresAndAtomics = VK_TRUE;		// 允许片段着色器计算buffer后进行存储，做后处理等工作时需要开启这个特性，让片段着色器能够存储buffer然后传递给后处理
		deviceFeatures.independentBlend = VK_TRUE;				// 开启独立混合，允许渲染目标自己进行颜色混合
		if (config.enablePointLightShadow) {
			deviceFeatures.geometryShader = VK_TRUE;			// 如果要支持点光源阴影计算，生成深度立方贴图需要开启几何着色器阶段
		}

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		if (config.enableValidation) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if ((result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicDevice)) != VK_SUCCESS) {
			LOG_ERROR("failed to create logical device!");
			return result;
		}

		vkGetDeviceQueue(logicDevice, indices.graphicsFamily, 0, &graphicsQueue);
		vkGetDeviceQueue(logicDevice, indices.presentFamily, 0, &presentQueue);

		// 这里创建几个后面会经常用的函数的指针，防止某些设备不支持调用导致crash
		_vkResetCommandPool = (PFN_vkResetCommandPool)vkGetDeviceProcAddr(logicDevice, "vkResetCommandPool");
		_vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer)vkGetDeviceProcAddr(logicDevice, "vkBeginCommandBuffer");
		_vkEndCommandBuffer = (PFN_vkEndCommandBuffer)vkGetDeviceProcAddr(logicDevice, "vkEndCommandBuffer");
		_vkCmdBeginRenderPass = (PFN_vkCmdBeginRenderPass)vkGetDeviceProcAddr(logicDevice, "vkCmdBeginRenderPass");
		_vkCmdNextSubpass = (PFN_vkCmdNextSubpass)vkGetDeviceProcAddr(logicDevice, "vkCmdNextSubpass");
		_vkCmdEndRenderPass = (PFN_vkCmdEndRenderPass)vkGetDeviceProcAddr(logicDevice, "vkCmdEndRenderPass");
		_vkCmdBindPipeline = (PFN_vkCmdBindPipeline)vkGetDeviceProcAddr(logicDevice, "vkCmdBindPipeline");
		_vkCmdSetViewport = (PFN_vkCmdSetViewport)vkGetDeviceProcAddr(logicDevice, "vkCmdSetViewport");
		_vkCmdSetScissor = (PFN_vkCmdSetScissor)vkGetDeviceProcAddr(logicDevice, "vkCmdSetScissor");
		_vkWaitForFences = (PFN_vkWaitForFences)vkGetDeviceProcAddr(logicDevice, "vkWaitForFences");
		_vkResetFences = (PFN_vkResetFences)vkGetDeviceProcAddr(logicDevice, "vkResetFences");
		_vkCmdDrawIndexed = (PFN_vkCmdDrawIndexed)vkGetDeviceProcAddr(logicDevice, "vkCmdDrawIndexed");
		_vkCmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers)vkGetDeviceProcAddr(logicDevice, "vkCmdBindVertexBuffers");
		_vkCmdBindIndexBuffer = (PFN_vkCmdBindIndexBuffer)vkGetDeviceProcAddr(logicDevice, "vkCmdBindIndexBuffer");
		_vkCmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets)vkGetDeviceProcAddr(logicDevice, "vkCmdBindDescriptorSets");
		_vkCmdClearAttachments = (PFN_vkCmdClearAttachments)vkGetDeviceProcAddr(logicDevice, "vkCmdClearAttachments");

		return result;
	}

	/*
	* 这里我们设计多个command Pool
	* 1. 一个可以被reset的pool用于专门给默认图形command buffer用
	* 2. n个command pool用于给其他图形操作用，每个线程一个，也就是和最大并行帧数相等
	*/
	VkResult VulkanRHI::createCommandPool() {
		VkResult result = VK_SUCCESS;
		QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice, surface);
		// default command pool
		VkCommandPoolCreateInfo defaultPoolInfo = {};
		defaultPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		defaultPoolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
		defaultPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		if ((result = vkCreateCommandPool(logicDevice, &defaultPoolInfo, nullptr, &defaultCommandPool)) != VK_SUCCESS) {
			LOG_ERROR("Create default command pool Falied");
			return result;
		}

		// other command pool
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
		for (uint8_t i = 0; i < maxFrameFlight; i++) {
			if ((result = vkCreateCommandPool(logicDevice, &defaultPoolInfo, nullptr, &commandPools[i])) != VK_SUCCESS) {
				LOG_ERROR("Create command idx: " + std::to_string(i) +  " pool Falied");
				return result;
			}
		}
		return VK_SUCCESS;
	}

	/*
	* 考虑多线程问题，我们给每个pool分配一个buffer即可
	*/
	VkResult VulkanRHI::createCommandBuffers() {
		VkResult result = VK_SUCCESS;
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1U;

		for (uint8_t i = 0; i < maxFrameFlight; i++) {
			allocInfo.commandPool = commandPools[i];
			if ((result = vkAllocateCommandBuffers(logicDevice, &allocInfo, &commandBuffers[i])) != VK_SUCCESS) {
				return result;
			}
		}
		return VK_SUCCESS;
	}
	
	VkResult VulkanRHI::createDescriptorPool() {
		// 创建7个池子，每个池子的功能不一样
		// todo: 这里有些参数配置是硬编码，暂时不知道为什么用这个数，后续需要了解一下
		VkResult result;
		VkDescriptorPoolSize poolSizes[7];
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
		poolSizes[0].descriptorCount = 3 + 2 + 2 + 2 + 1 + 1 + 3 + 3;
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		poolSizes[1].descriptorCount = 1 + 1 + 1 * maxVertexBlendingMeshCount;
		poolSizes[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[2].descriptorCount = maxMaterialCount;
		poolSizes[3].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[3].descriptorCount = 3 + 5 * maxMaterialCount + 1 + 1;
		poolSizes[4].type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		poolSizes[4].descriptorCount = 4 + 1 + 1 + 2;
		poolSizes[5].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		poolSizes[5].descriptorCount = 3;
		poolSizes[6].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		poolSizes[6].descriptorCount = 1;

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		// pool size == 7
		poolInfo.poolSizeCount = sizeof(poolSizes) / sizeof(poolSizes[0]);
		poolInfo.pPoolSizes = poolSizes;
		poolInfo.maxSets = 1 + 1 + 1 + maxMaterialCount + maxVertexBlendingMeshCount + 1 + 1; // +skybox + axis descriptor set
		poolInfo.flags = 0U;

		if ((result = vkCreateDescriptorPool(logicDevice, &poolInfo, nullptr, &descriptorPool)) != VK_SUCCESS)
		{
			return result;
		}
		return VK_SUCCESS;
	}

	VkResult VulkanRHI::createSyncObjects() {
		// 信号量创建三个
		/*
		* 1. 阻塞image可获取前
		* 2. 阻塞renderpass执行完毕前
		* 3. 阻塞纹理复制完成前
		*/
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < maxFrameFlight; i++) {
			if (vkCreateSemaphore(logicDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS
				|| vkCreateSemaphore(logicDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS
				|| vkCreateSemaphore(logicDevice, &semaphoreInfo, nullptr, &imageAvailableTexturescopySemaphores[i]) != VK_SUCCESS
				|| vkCreateFence(logicDevice, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
				return VK_SUCCESS;
			}
		}
		return VK_SUCCESS;
	}

	VkResult VulkanRHI::createSwapChain() {
		VkResult result;
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);
		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
		uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily };
		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;
		if ((result = vkCreateSwapchainKHR(logicDevice, &createInfo, nullptr, &swapChain)) != VK_SUCCESS) {
			return result;
		}
		vkGetSwapchainImagesKHR(logicDevice, swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(logicDevice, swapChain, &imageCount, swapChainImages.data());
		swapchainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;
		return VK_SUCCESS;
	}

	VkResult VulkanRHI::createImageViews() {
		swapChainImageViews.resize(swapChainImages.size());
		for (uint32_t i = 0; i < swapChainImages.size(); i++) {
			VkImageView_t imageView_t = createImageView(
				logicDevice,
				swapChainImages[i],
				swapchainImageFormat,
				VK_IMAGE_ASPECT_COLOR_BIT,
				VK_IMAGE_VIEW_TYPE_2D,
				1,
				1
			);
			if (imageView_t.result != VK_SUCCESS) {
				return imageView_t.result;
			}else {
				swapChainImageViews[i] = imageView_t.imageView;
			}
		}
		return VK_SUCCESS;
	}

	VkResult VulkanRHI::createDepthResources() {
		VkResult result;
		VkFormat_t depthFormat_t = findDepthFormat(physicalDevice);
		if ((result = depthFormat_t.result) != VK_SUCCESS) {
			return result;
		}
		createImage(
			physicalDevice,
			logicDevice,
			swapChainExtent.width,
			swapChainExtent.height,
			depthFormat_t.format,
			VK_IMAGE_TILING_OPTIMAL, 
			VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			depthImage,
			depthImageMemory,
			0,
			1,
			1,
			VK_SAMPLE_COUNT_1_BIT
		);
		VkImageView_t depthImageView_t = createImageView(logicDevice, depthImage, depthFormat_t.format, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_VIEW_TYPE_2D, 1, 1);
		if (depthImageView_t.result != VK_SUCCESS) {
			return depthImageView_t.result;
		}
		depthImageView = depthImageView_t.imageView;
		return VK_SUCCESS;
	}

	VkResult VulkanRHI::createSampler(const VkSamplerCreateInfo* pCreateInfo, VkSampler& sampler) {
        return vkCreateSampler(logicDevice, pCreateInfo, nullptr, &sampler);
	}

	// 使用vma管理内存
	VkResult VulkanRHI::createAssetAllocator() {
		VmaVulkanFunctions vulkanFunctions = {};
		vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
		vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

		VmaAllocatorCreateInfo allocatorCreateInfo = {};
		allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_0;
		allocatorCreateInfo.physicalDevice = physicalDevice;
		allocatorCreateInfo.device = logicDevice;
		allocatorCreateInfo.instance = instance;
		allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

		return vmaCreateAllocator(&allocatorCreateInfo, &assetsAllocator);
	}

	VkResult VulkanRHI::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
		VkResult result;
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if ((result = vkCreateBuffer(logicDevice, &bufferInfo, nullptr, &buffer)) != VK_SUCCESS) {
			return result;
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(logicDevice, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);
		if ((result = vkAllocateMemory(logicDevice, &allocInfo, nullptr, &bufferMemory)) != VK_SUCCESS) {
			return result;
		}
		vkBindBufferMemory(logicDevice, buffer, bufferMemory, 0);
		return VK_SUCCESS;
    }

	void VulkanRHI::createCubeMap(
		VkImage& image, 
		VkImageView& imageView, 
		VmaAllocation& imageAllocation, 
		uint32_t textureImageWidth, 
		uint32_t textureImageHeight, 
		std::array<void*, 6> textureImagePixels, 
		VkFormat textureImageFormat, 
		uint32_t miplevels
	) {
		VkDeviceSize textureLayerByteSize;
        VkDeviceSize cubeByteSize;
        VkFormat     vulkanImageFormat;
        switch (textureImageFormat) {
            case VkFormat::VK_FORMAT_R8G8B8_UNORM:
                textureLayerByteSize = textureImageWidth * textureImageHeight * 3;
                vulkanImageFormat = VK_FORMAT_R8G8B8_UNORM;
                break;
            case VkFormat::VK_FORMAT_R8G8B8_SRGB:
                textureLayerByteSize = textureImageWidth * textureImageHeight * 3;
                vulkanImageFormat = VK_FORMAT_R8G8B8_SRGB;
                break;
            case VkFormat::VK_FORMAT_R8G8B8A8_UNORM:
                textureLayerByteSize = textureImageWidth * textureImageHeight * 4;
                vulkanImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
                break;
            case VkFormat::VK_FORMAT_R8G8B8A8_SRGB:
                textureLayerByteSize = textureImageWidth * textureImageHeight * 4;
                vulkanImageFormat = VK_FORMAT_R8G8B8A8_SRGB;
                break;
            case VkFormat::VK_FORMAT_R32G32_SFLOAT:
                textureLayerByteSize = textureImageWidth * textureImageHeight * 4 * 2;
                vulkanImageFormat = VK_FORMAT_R32G32_SFLOAT;
                break;
            case VkFormat::VK_FORMAT_R32G32B32_SFLOAT:
                textureLayerByteSize = textureImageWidth * textureImageHeight * 4 * 3;
                vulkanImageFormat = VK_FORMAT_R32G32B32_SFLOAT;
                break;
            case VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT:
                textureLayerByteSize = textureImageWidth * textureImageHeight * 4 * 4;
                vulkanImageFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
                break;
            default:
                textureLayerByteSize = VkDeviceSize(-1);
                LOG_ERROR("invalid textureLayerByteSize");
                return;
        }
        // cube map有6个面
        cubeByteSize = textureLayerByteSize * 6;

        // create cubemap texture image
        VkImageCreateInfo imageCreateInfo {};
        imageCreateInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.flags         = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
        imageCreateInfo.imageType     = VK_IMAGE_TYPE_2D;
        imageCreateInfo.extent.width  = static_cast<uint32_t>(textureImageWidth);
        imageCreateInfo.extent.height = static_cast<uint32_t>(textureImageHeight);
        imageCreateInfo.extent.depth  = 1;
        imageCreateInfo.mipLevels     = miplevels;
        imageCreateInfo.arrayLayers   = 6;
        imageCreateInfo.format        = vulkanImageFormat;
        imageCreateInfo.tiling        = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageCreateInfo.usage 		= VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        imageCreateInfo.samples     = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage                   = VMA_MEMORY_USAGE_GPU_ONLY;

        // vma管理创建好的image的内存分配
        VkResult result = vmaCreateImage(assetsAllocator, &imageCreateInfo, &allocInfo, &image, &imageAllocation, NULL);
		if (result != VK_SUCCESS) {
			std::cout << "create image failed\n";
		}

        VkBuffer       inefficientStagingBuffer;
        VkDeviceMemory inefficientStagingBufferMemory;
        createBuffer(
			cubeByteSize, 
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
			inefficientStagingBuffer, 
			inefficientStagingBufferMemory
		);

        // 6个面按照offset都绑定到data上供cpu端操作，通过 inefficientStagingBufferMemory 把纹理数据拷贝到data中
        void* data = NULL;
        vkMapMemory(logicDevice, inefficientStagingBufferMemory, 0, cubeByteSize, 0, &data);
        for (int i=0;i<6;i++) {
            memcpy((void*)(static_cast<char*>(data) + textureLayerByteSize * i), textureImagePixels[i], static_cast<size_t>(textureLayerByteSize));
        }
        // inefficientStagingBufferMemory 用完了可以卸载了
        vkUnmapMemory(logicDevice, inefficientStagingBufferMemory);

        // 转换布局，把image转换到renderpass的dest布局
        transitionImageLayout(
			this, 
			image,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            6,
            miplevels,
            VK_IMAGE_ASPECT_COLOR_BIT
		);
        // 把inefficient_staging_buffer拷贝到image，这一步之后image就算搞定了
        copyBufferToImage(
			this,
            inefficientStagingBuffer,
            image,
            static_cast<uint32_t>(textureImageWidth),
            static_cast<uint32_t>(textureImageHeight),
            6
		);
        // image搞定了，前面的所有buffer都可以释放了
        vkDestroyBuffer(logicDevice, inefficientStagingBuffer, nullptr);
        vkFreeMemory(logicDevice, inefficientStagingBufferMemory, nullptr);

		// 创建mipmap纹理
        generateMipmapsTexture(this, image, vulkanImageFormat, textureImageWidth, textureImageHeight, 6, miplevels);
        // 最后创建imageview
        imageView = createImageView(logicDevice, image, vulkanImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_CUBE, 6, miplevels).imageView;
    }

	void VulkanRHI::createGlobalImage(
		VkImage& image,
        VkImageView& imageView,
        VmaAllocation& imageAllocation,
        uint32_t textureImageWidth,
        uint32_t textureImageHeight,
    	void* textureImagePixels,
        VkFormat textureImageFormat,
        uint32_t miplevels
	) {
        VkDeviceSize textureByteSize;
        VkFormat     vulkanImageFormat;
        switch (textureImageFormat)
        {
            case VkFormat::VK_FORMAT_R8G8B8_UNORM:
                textureByteSize   = textureImageWidth * textureImageHeight * 3;
                vulkanImageFormat = VK_FORMAT_R8G8B8_UNORM;
                break;
            case VkFormat::VK_FORMAT_R8G8B8_SRGB:
                textureByteSize   = textureImageWidth * textureImageHeight * 3;
                vulkanImageFormat = VK_FORMAT_R8G8B8_SRGB;
                break;
            case VkFormat::VK_FORMAT_R8G8B8A8_UNORM:
                textureByteSize   = textureImageWidth * textureImageHeight * 4;
                vulkanImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
                break;
            case VkFormat::VK_FORMAT_R8G8B8A8_SRGB:
                textureByteSize   = textureImageWidth * textureImageHeight * 4;
                vulkanImageFormat = VK_FORMAT_R8G8B8A8_SRGB;
                break;
            case VkFormat::VK_FORMAT_R32_SFLOAT:
                textureByteSize = textureImageWidth * textureImageHeight * 4;
                vulkanImageFormat = VK_FORMAT_R32_SFLOAT;
                break;
            case VkFormat::VK_FORMAT_R32G32_SFLOAT:
                textureByteSize   = textureImageWidth * textureImageHeight * 4 * 2;
                vulkanImageFormat = VK_FORMAT_R32G32_SFLOAT;
                break;
            case VkFormat::VK_FORMAT_R32G32B32_SFLOAT:
                textureByteSize   = textureImageWidth * textureImageHeight * 4 * 3;
                vulkanImageFormat = VK_FORMAT_R32G32B32_SFLOAT;
                break;
            case VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT:
                textureByteSize   = textureImageWidth * textureImageHeight * 4 * 4;
                vulkanImageFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
                break;
            default:
                LOG_ERROR("invalid textureByteSize");
                break;
        }

        VkBuffer       inefficientStagingBuffer;
        VkDeviceMemory inefficientStagingBufferMemory;
        createBuffer(
            textureByteSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            inefficientStagingBuffer,
            inefficientStagingBufferMemory
		);

        void* data;
        vkMapMemory(logicDevice, inefficientStagingBufferMemory, 0, textureByteSize, 0, &data);
        memcpy(data, textureImagePixels, static_cast<size_t>(textureByteSize));
        vkUnmapMemory(logicDevice, inefficientStagingBufferMemory);

        uint32_t mip_levels = (miplevels != 0) ? miplevels : floor(log2(std::max(textureImageWidth, textureImageHeight))) + 1;

        VkImageCreateInfo imageCreateInfo {};
        imageCreateInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.flags         = 0;
        imageCreateInfo.imageType     = VK_IMAGE_TYPE_2D;
        imageCreateInfo.extent.width  = textureImageWidth;
        imageCreateInfo.extent.height = textureImageHeight;
        imageCreateInfo.extent.depth  = 1;
        imageCreateInfo.mipLevels     = mip_levels;
        imageCreateInfo.arrayLayers   = 1;
        imageCreateInfo.format        = vulkanImageFormat;
        imageCreateInfo.tiling        = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageCreateInfo.usage 		= VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        imageCreateInfo.samples     = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

        vmaCreateImage(assetsAllocator, &imageCreateInfo, &allocInfo, &image, &imageAllocation, NULL);

        transitionImageLayout(
			this,
            image,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            1,
            VK_IMAGE_ASPECT_COLOR_BIT
		);
        copyBufferToImage(this, inefficientStagingBuffer, image, textureImageWidth, textureImageHeight, 1);
        transitionImageLayout(
			this,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            1,
            1,
            VK_IMAGE_ASPECT_COLOR_BIT
		);

        vkDestroyBuffer(logicDevice, inefficientStagingBuffer, nullptr);
        vkFreeMemory(logicDevice, inefficientStagingBufferMemory, nullptr);

        // generate mipmapped image, not texture
        generateMipmapsImage(this, image, textureImageWidth, textureImageHeight, mip_levels);

        imageView = createImageView(
			logicDevice,
            image,
            vulkanImageFormat,
            VK_IMAGE_ASPECT_COLOR_BIT,
            VK_IMAGE_VIEW_TYPE_2D,
            1,
        	mip_levels
		).imageView;
	}

	// Get
	VkPhysicalDeviceProperties VulkanRHI::getPhysicalDeviceProperties() {
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);
		return properties;
	}

	VkResult VulkanRHI::mapMemory(VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) {
		return vkMapMemory(logicDevice, memory, offset, size, flags, ppData);
	}

	// Destory
	void VulkanRHI::destroyDebugUtilsMessengerEXT(VkInstance instance,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT callback) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, callback, pAllocator);
		}
	}

	void VulkanRHI::destroySemaphores(VkSemaphore semaphores[]) {
		for (size_t i = 0; i < maxFrameFlight; i++) {
			vkDestroySemaphore(logicDevice, semaphores[i], nullptr);
		}
	}

	void VulkanRHI::destroyFences(VkFence fences[]) {
		for (size_t i = 0; i < maxFrameFlight; i++) {
			vkDestroyFence(logicDevice, fences[i], nullptr);
		}
	}

	void VulkanRHI::destroySampler(VkSampler sampler) {
		vkDestroySampler(logicDevice, sampler, nullptr);
	}

	// other
	VkCommandBuffer VulkanRHI::beginSingleTimeCommands() {
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = defaultCommandPool;
		allocInfo.commandBufferCount = 1;
		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(logicDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		_vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VulkanRHI::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
		_vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);
		vkFreeCommandBuffers(logicDevice, defaultCommandPool, 1, &commandBuffer);
	}

	// Initiative
	bool VulkanRHI::initiative(RenderInterface interface, VulkanConfig config) {
		this->window = interface.window_system->window;
		this->config = config;
		VkResult err;

		err = createInstance();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Could not create Vulkan instance! err code [ " + VkErrorMsg[err] + " ]");
			return false;
		}else {
			LOG_INFO("Create Vulkan Instance Success!")
		}

		err = createWindowSurface();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Failed to Create Window Surface! err code [ " + VkErrorMsg[err] + " ]");
			return false;
		}else {
			LOG_INFO("Create Window Surface Success!");
		}

		err = pickPhysicalDevice();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Failed to find a compatible Device! err code [ " + VkErrorMsg[err] + " ]");
			return false;
		}else {
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(physicalDevice, &properties);
			LOG_INFO("Pick a compatible Device: " + properties.deviceName);
		}

		err = createLogicalDevice();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Failed to create a logicDevice! err code [ " + VkErrorMsg[err] + " ]");
			return false;
		}else {
			LOG_INFO("Create logicDevice Success!");
		}
		
		err = createCommandPool();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Failed to create a Command Pool! err code [ " + VkErrorMsg[err] + " ]");
			return false;
		}else {
			LOG_INFO("Create Command Pool Success!");
		}
		
		err = createCommandBuffers();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Failed to create a Command Buffers! err code [ " + VkErrorMsg[err] + " ]");
			return false;
		}else {
			LOG_INFO("Create Command Buffers Success!");
		}

		err = createDescriptorPool();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Create Descriptor Pool Failed! err code [ " + VkErrorMsg[err] + " ]");
			return false;
		}else {
			LOG_INFO("Create Descriptor Pool Success!");
		}

		err = createSyncObjects();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Create Sync Objects Failed! err code [ " + VkErrorMsg[err] + " ]");
			return false;
		}else {
			LOG_INFO("Create Sync Objects Success!");
		}

		err = createSwapChain();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Create SwapChain Failed! err code [ " + VkErrorMsg[err] + " ]");
			return false;
		}else {
			LOG_INFO("Create SwapChain Success!");
		}

		err = createImageViews();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Create SwapChain ImageViews Failed! err code [ " + VkErrorMsg[err] + " ]");
			return false;
		}else {
			LOG_INFO("Create SwapChain ImageViews Success!");
		}

		err = createDepthResources();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Create Depth Resources Failed! err code [ " + VkErrorMsg[err] + " ]");
			return false;
		}else {
			LOG_INFO("Create Depth Resources Success!");
		}

		err = createAssetAllocator();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Create Vma Failed! err code [ " + VkErrorMsg[err] + " ]");
			return false;
		}else {
			LOG_INFO("Create Vma Success!");
		}

		// createRenderPass();
		// createDescriptorSetLayout();
		// createGraphicsPipeline();
		// 配置多重采样的颜色和深度资源
		// createColorResources();
		// 创建帧缓冲, 如果使用深度图，则帧缓冲的创建依赖深度图，需要在创建深度图资源后再创建帧缓冲
		// createFramebuffers();
		// 加载图像数据到vulkan实例中，因为要使用指令缓冲，所以这一步需要在createCommandPool之后
		// createTextureImage();
		// 创建上一步纹理图像的视图
		// createTextureImageView();
		// 创建采样器(如果不使用纹理的话这一步可以不要)
		// createTextureSampler();
		// 如果我们需要渲染模型，在这里创建模型顶点和索引
		// loadModel();
		// 创建顶点缓冲
		// createVertexBuffer();
		// 创建索引缓冲
		// createIndexBuffer();
		// 创建描述符缓冲
		// createUniformBuffer();
		// 创建描述符集对象
		// createDescriptorSets();
		return true;
	}

	void VulkanRHI::destroy() {
		destroySemaphores(renderFinishedSemaphores);
		destroySemaphores(imageAvailableSemaphores);
		destroySemaphores(imageAvailableTexturescopySemaphores);
		destroyFences(inFlightFences);
		LOG_INFO("Destory Sync Objects Success!");

		for (uint8_t i = 0; i < maxFrameFlight; i++) {
			vkDestroyCommandPool(logicDevice, commandPools[i], nullptr);
		}
		vkDestroyCommandPool(logicDevice, defaultCommandPool, nullptr);
		LOG_INFO("Destory Command Pool Success!");

		vkDestroyImageView(logicDevice, depthImageView, nullptr);
		vkDestroyImage(logicDevice, depthImage, nullptr);
		vkFreeMemory(logicDevice, depthImageMemory, nullptr);
		LOG_INFO("Destory Depth Resources Success!");

		for (VkImageView imageView : swapChainImageViews) {
			vkDestroyImageView(logicDevice, imageView, nullptr);
		}
		LOG_INFO("Destory ImageViews Success!");

		vkDestroySwapchainKHR(logicDevice, swapChain, nullptr);
		LOG_INFO("Destory SwapChain Success!");

		vkDestroyDescriptorPool(logicDevice, descriptorPool, nullptr);
		LOG_INFO("Destory Descriptor Pool Success!");

		vkDestroyDevice(logicDevice, nullptr);
		LOG_INFO("Destory Logic Device Success!");

		// 如果开启了debug模式，销毁debug messenger
		if (config.enableValidation) {
			destroyDebugUtilsMessengerEXT(instance, nullptr, callback);
			LOG_INFO("Destory Debug Messenger Success!");
		}
		vkDestroySurfaceKHR(instance, surface, nullptr);
		LOG_INFO("Destory Surface Handler Success!");

		vkDestroyInstance(instance, nullptr);
		LOG_INFO("Destory Vulkan Instance Success!");

		glfwDestroyWindow(window);
		glfwTerminate();
		LOG_INFO("Destory GLFW Window Success!");

		LOG_INFO("Destory Vulkan Handler Success!");
	}

	VulkanRHI::~VulkanRHI() {
		destroy();
	}
}