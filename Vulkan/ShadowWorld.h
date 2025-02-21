#pragma once
#include "Vulkan.h"
#include "Device.h"
namespace SEVIAN {
	namespace VULKAN {
		class ShadowWorld
		{

			uint32_t width;
			uint32_t height;

			VkFramebuffer createShadowFramebuffer ( VkRenderPass shadowRenderPass, VkImageView depthImageView, uint32_t width, uint32_t height );
		public:

			ShadowWorld ( Device* device, uint32_t width, uint32_t height );


			VkRenderPass renderPass = VK_NULL_HANDLE;
			Device* device = VK_NULL_HANDLE;

			VkImageView imageView = VK_NULL_HANDLE;
			VkImage image = VK_NULL_HANDLE;
			VkDeviceMemory imageMemory = VK_NULL_HANDLE;

			VkFramebuffer frameBuffer = VK_NULL_HANDLE;
		};


	}


}