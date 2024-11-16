#include "PhysicalDevice.h"


namespace VULKAN {
	PhysicalDevice::PhysicalDevice ( VkInstance _instance, VkSurfaceKHR _surface, VkPhysicalDevice _physicalDevice ) {
		instance = _instance;
		surface = _surface;
		physicalDevice = _physicalDevice;

	}

	void PhysicalDevice::findQueueFamilies () {


		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties ( physicalDevice, &queueFamilyCount, nullptr );

		std::vector<VkQueueFamilyProperties> queueFamilies ( queueFamilyCount );

		vkGetPhysicalDeviceQueueFamilyProperties ( physicalDevice, &queueFamilyCount, queueFamilies.data () );

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR ( physicalDevice, i, surface, &presentSupport );

			if (presentSupport) {
				presentFamily = i;
			}

			if (isComplete ()) {
				break;
			}

			i++;
		}


	}

	VkImageView PhysicalDevice::createImageView ( VkImage image, VkFormat format ) {
		VkImageViewCreateInfo viewInfo {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		viewInfo.components = {
			VK_COMPONENT_SWIZZLE_R,
			VK_COMPONENT_SWIZZLE_G,
			VK_COMPONENT_SWIZZLE_B,
			VK_COMPONENT_SWIZZLE_A
		};

		VkImageView imageView;
		if (vkCreateImageView ( device, &viewInfo, nullptr, &imageView ) != VK_SUCCESS) {
			throw std::runtime_error ( "failed to create texture image view!" );
		}

		return imageView;
	}

	void PhysicalDevice::createLogicalDevice () {

		// Enumerar las extensiones disponibles en el dispositivo físico
		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties ( physicalDevice, nullptr, &extensionCount, nullptr );
		std::vector<VkExtensionProperties> availableExtensions ( extensionCount );
		vkEnumerateDeviceExtensionProperties ( physicalDevice, nullptr, &extensionCount, availableExtensions.data () );

		// Imprimir las extensiones disponibles
		for (const auto& extension : availableExtensions) {
			std::cout << "Extension: " << extension.extensionName << std::endl;
		}

		//QueueFamilyIndices indices = 
		findQueueFamilies ();

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		std::set<uint32_t> uniqueQueueFamilies = { graphicsFamily.value (), presentFamily.value () };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back ( queueCreateInfo );
		}

		VkPhysicalDeviceFeatures deviceFeatures {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo createInfo {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size ());
		createInfo.pQueueCreateInfos = queueCreateInfos.data ();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size ());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data ();

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size ());
			createInfo.ppEnabledLayerNames = validationLayers.data ();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice ( physicalDevice, &createInfo, nullptr, &device ) != VK_SUCCESS) {
			throw std::runtime_error ( "failed to create logical device!" );
		}
		printf ( "que es un device---> (%p)\n", device );
		vkGetDeviceQueue ( device, graphicsFamily.value (), 0, &graphicsQueue );
		vkGetDeviceQueue ( device, presentFamily.value (), 0, &presentQueue );
	}

	void PhysicalDevice::querySwapChainSupport () {


		vkGetPhysicalDeviceSurfaceCapabilitiesKHR ( physicalDevice, surface, &capabilities );

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR ( physicalDevice, surface, &formatCount, nullptr );

		if (formatCount != 0) {
			formats.resize ( formatCount );
			vkGetPhysicalDeviceSurfaceFormatsKHR ( physicalDevice, surface, &formatCount, formats.data () );
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR ( physicalDevice, surface, &presentModeCount, nullptr );

		if (presentModeCount != 0) {
			presentModes.resize ( presentModeCount );
			vkGetPhysicalDeviceSurfacePresentModesKHR ( physicalDevice, surface, &presentModeCount, presentModes.data () );
		}


	}

	SwapChain PhysicalDevice::createSwapChain ( GLFWwindow* window ) {


		//SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
		querySwapChainSupport ();
		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat ( formats );
		VkPresentModeKHR presentMode = chooseSwapPresentMode ( presentModes );
		VkExtent2D extent = chooseSwapExtent ( capabilities, window );

		uint32_t imageCount = capabilities.minImageCount + 1;
		if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
			imageCount = capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT /* | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT*/;

		findQueueFamilies ();
		uint32_t queueFamilyIndices[] = { graphicsFamily.value (), presentFamily.value () };

		if (graphicsFamily != presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		createInfo.preTransform = capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;




		SwapChain swapChain;


		//VkSwapchainKHR swapChain;
		if (vkCreateSwapchainKHR ( device, &createInfo, nullptr, &swapChain.swapchain ) != VK_SUCCESS) {
			throw std::runtime_error ( "failed to create swap chain!" );
		}

		vkGetSwapchainImagesKHR ( device, swapChain.swapchain, &imageCount, nullptr );
		swapChain.swapChainImages.resize ( imageCount );
		vkGetSwapchainImagesKHR ( device, swapChain.swapchain, &imageCount, swapChain.swapChainImages.data () );

		swapChain.imageFormat = surfaceFormat.format;
		swapChain.extent = extent;
		swapChain.device = device;
		swapChain.imageViews.resize ( swapChain.swapChainImages.size () );

		for (size_t i = 0; i < swapChain.swapChainImages.size (); i++) {
			swapChain.imageViews[i] = createImageView ( swapChain.swapChainImages[i], swapChain.imageFormat );
		}


		return swapChain;

	}

	std::vector<VkFramebuffer> PhysicalDevice::createFramebuffers ( SwapChain swapChain, VkRenderPass renderPass, std::vector<VkImageView> att ) {

		std::vector<VkFramebuffer> swapChainFramebuffers;


		swapChainFramebuffers.resize ( swapChain.imageViews.size () );

		for (size_t i = 0; i < swapChain.imageViews.size (); i++) {

			std::vector<VkImageView> attachments;
			attachments.push_back ( swapChain.imageViews[i] );
			attachments.push_back ( att[0] );
			/*
			VkImageView attachments[] = {
					swapChain.imageViews[i]
			};
			*/
			VkFramebufferCreateInfo framebufferInfo {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.pNext = NULL;
			framebufferInfo.renderPass = renderPass;
			//framebufferInfo.attachmentCount = 1;// static_cast<uint32_t>(attachments.size());
			//framebufferInfo.pAttachments = attachments;

			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size ());
			framebufferInfo.pAttachments = attachments.data ();

			framebufferInfo.width = swapChain.extent.width;
			framebufferInfo.height = swapChain.extent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer ( device, &framebufferInfo, nullptr, &swapChainFramebuffers[i] ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to create framebuffer!" );
			}
		}

		return swapChainFramebuffers;
	}

	VkFramebuffer PhysicalDevice::createShadowFramebuffer ( VkRenderPass shadowRenderPass, VkImageView depthImageView, VkExtent2D shadowExtent ) {

		// Configuración de la información del framebuffer
		VkFramebufferCreateInfo framebufferInfo {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = shadowRenderPass;  // Render pass específico para sombras
		framebufferInfo.attachmentCount = 1;  // Solo 1 attachment (depth)
		framebufferInfo.pAttachments = &depthImageView;  // El attachment es el depth image view

		framebufferInfo.width = shadowExtent.width;   // Tamaño del framebuffer para sombras
		framebufferInfo.height = shadowExtent.height; // Tamaño del framebuffer para sombras
		framebufferInfo.layers = 1;  // Solo una capa, a menos que estés trabajando con un shadow map en 3D

		VkFramebuffer shadowFramebuffer;
		if (vkCreateFramebuffer ( device, &framebufferInfo, nullptr, &shadowFramebuffer ) != VK_SUCCESS) {
			throw std::runtime_error ( "failed to create shadow framebuffer!" );
		}

		return shadowFramebuffer;
	}

	VkCommandPool PhysicalDevice::createCommandPool () {

		VkCommandPool commandPool;


		//QueueFamilyIndices queueFamilyIndices =
		//	findQueueFamilies();

		VkCommandPoolCreateInfo poolInfo {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = graphicsFamily.value ();

		if (vkCreateCommandPool ( device, &poolInfo, nullptr, &commandPool ) != VK_SUCCESS) {
			throw std::runtime_error ( "failed to create graphics command pool!" );
		}

		return commandPool;
	}

	Device* PhysicalDevice::createDevice () {
		createLogicalDevice ();
		Device* devic = new Device ( physicalDevice, device, graphicsQueue );
		return devic;
	}

}


