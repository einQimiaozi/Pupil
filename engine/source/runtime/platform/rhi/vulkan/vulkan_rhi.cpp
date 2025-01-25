#include "vulkan_rhi.h"
#include "core/macro.h"
#include "platform/rhi/vulkan/vulkan_utils.h"

namespace Pupil {
	VkResult VulkanRHI::createInstance() {
		std::string version = getVulkanVersion();
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

		// ��ȡ��Ҫ��չ
		std::vector<const char*> extensions = getRequiredExtensions(config.enableValidation);
		// ���glfw����չ�Ƿ�����ڵ�ǰ�豸֧�ֵ���չ��
		if (config.enableValidation && !checkExtensionSupport(extensionsName)) {
			LOG_ERROR("extenstions requested, but not available!");
			return VK_ERROR_INITIALIZATION_FAILED;
		}
		// �����֤���Ƿ�����ڵ�ǰ�豸֧�ֵĲ�
		if (config.enableValidation && !checkValidationLayerSupport(validationLayers)) {
			LOG_ERROR("layers requested, but not available!");
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		// ��ʼ��ʵ��������Ϣ�е�У������չ��Ϣ������У���ʹ��
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

		// ����vulkanʵ��
		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
		if (result != VK_SUCCESS) {
			return result;
		}

		// ע��debugmessager�ص�
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
		// ʹ��CreateDebugUtilsMessengerEXT����debug messenger
		if ((result = CreateDebugUtilsMessengerEXT(instance, &debugMessengerCreateInfo, nullptr, &callback)) != VK_SUCCESS) {
			return result;
		}
		return VK_SUCCESS;
	}

	// ����debug messenger
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
			// ���㵱ǰӲ���豸�÷�
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
		// ���û�ҵ����������豸���׳��쳣
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

		// ����Ӳ������, ��������Ӳ������������д
		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		if (config.enableRateShading) {
			deviceFeatures.sampleRateShading = VK_TRUE;
		}
		deviceFeatures.fragmentStoresAndAtomics = VK_TRUE;		// ����Ƭ����ɫ������buffer����д洢��������ȹ���ʱ��Ҫ����������ԣ���Ƭ����ɫ���ܹ��洢bufferȻ�󴫵ݸ�����
		deviceFeatures.independentBlend = VK_TRUE;				// ����������ϣ�������ȾĿ���Լ�������ɫ���
		if (config.enablePointLightShadow) {
			deviceFeatures.geometryShader = VK_TRUE;			// ���Ҫ֧�ֵ��Դ��Ӱ���㣬�������������ͼ��Ҫ����������ɫ���׶�
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

		// ���ﴴ����������ᾭ���õĺ�����ָ�룬��ֹĳЩ�豸��֧�ֵ��õ���crash
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
	* ����������ƶ��command Pool
	* 1. һ�����Ա�reset��pool����ר�Ÿ�Ĭ��ͼ��command buffer��
	* 2. n��command pool���ڸ�����ͼ�β����ã�ÿ���߳�һ����Ҳ���Ǻ������֡�����
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
	* ���Ƕ��߳����⣬���Ǹ�ÿ��pool����һ��buffer����
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

	void VulkanRHI::DestroyDebugUtilsMessengerEXT(VkInstance instance,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT callback) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, callback, pAllocator);
		}
	}

	void VulkanRHI::DestroySwapChain() {
		// todo
	}

	bool VulkanRHI::initiative(VulkanInterface interface, VulkanConfig config) {
		this->window = interface.window_system->window;
		this->config = config;
		VkResult err;

		err = createInstance();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Could not create Vulkan instance, err msg [ " + VkErrorMsg[err] + " ]");
			return false;
		}else {
			LOG_INFO("Create Vulkan Instance Success!")
		}

		err = createWindowSurface();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Failed to Create Window Surface!, err msg [ " + VkErrorMsg[err] + " ]");
			return false;
		}else {
			LOG_INFO("Create Window Surface Success!");
		}

		err = pickPhysicalDevice();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Failed to find a compatible Device!, err msg [ " + VkErrorMsg[err] + " ]");
			return false;
		}else {
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(physicalDevice, &properties);
			LOG_INFO("Pick a compatible Device: " + properties.deviceName);
		}

		err = createLogicalDevice();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Failed to create a logicDevice!, err msg [ " + VkErrorMsg[err] + " ]");
			return false;
		} else {
			LOG_INFO("Create logicDevice Success!");
		}
		
		err = createCommandPool();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Failed to create a Command Pool!, err msg [ " + VkErrorMsg[err] + " ]");
			return false;
		}
		else {
			LOG_INFO("Create Command Pool Success!");
		}
		
		err = createCommandBuffers();
		if (err != VK_SUCCESS) {
			LOG_ERROR("Failed to create a Command Buffers!, err msg [ " + VkErrorMsg[err] + " ]");
			return false;
		}
		else {
			LOG_INFO("Create Command Buffers Success!");
		}
		// createImageViews();
		// createRenderPass();
		// createDescriptorSetLayout();
		// createGraphicsPipeline();
		// createSwapChain();
		// createCommandPool();
		// ���ö��ز�������ɫ�������Դ
		// createColorResources();
		// �������ͼ����Ҫ����Դ
		// createDepthResources();
		// ����֡����, ���ʹ�����ͼ����֡����Ĵ����������ͼ����Ҫ�ڴ������ͼ��Դ���ٴ���֡����
		// createFramebuffers();
		// ����ͼ�����ݵ�vulkanʵ���У���ΪҪʹ��ָ��壬������һ����Ҫ��createCommandPool֮��
		// createTextureImage();
		// ������һ������ͼ�����ͼ
		// createTextureImageView();
		// ����������(�����ʹ������Ļ���һ�����Բ�Ҫ)
		// createTextureSampler();
		// ���������Ҫ��Ⱦģ�ͣ������ﴴ��ģ�Ͷ��������
		// loadModel();
		// �������㻺��
		// createVertexBuffer();
		// ������������
		// createIndexBuffer();
		// ��������������
		// createUniformBuffer();
		// ������������Դ��
		// createDescriptorPool();
		// ����������������
		// createDescriptorSets();
		// ��ʼ��ͬ������
		// createSyncObjects();
		return true;
	}

	void VulkanRHI::destroy() {
		for (uint8_t i = 0; i < maxFrameFlight; i++) {
			vkDestroyCommandPool(logicDevice, commandPools[i], nullptr);
		}
		vkDestroyCommandPool(logicDevice, defaultCommandPool, nullptr);
		vkDestroyDevice(logicDevice, nullptr);
		// ���������debugģʽ������debug messenger
		if (config.enableValidation) {
			DestroyDebugUtilsMessengerEXT(instance, nullptr, callback);
		}
		vkDestroySurfaceKHR(instance, surface, nullptr);
		vkDestroyInstance(instance, nullptr);
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	VulkanRHI::~VulkanRHI() {
		destroy();
	}
}