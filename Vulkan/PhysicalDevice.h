#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <stdexcept>
#include "Vulkan.h"
#include "SwapChain.h"
#include "Device.h"
namespace SEVIAN {
	namespace VULKAN {

		const std::vector<const char*> deviceExtensions = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		class PhysicalDevice
		{
		private:
#ifdef NDEBUG
			const bool enableValidationLayers = false;
#else
			const bool enableValidationLayers = true;
#endif
			GLFWwindow* window;
			VkDevice device = VK_NULL_HANDLE;
			VkInstance instance = VK_NULL_HANDLE;
			//VkDebugUtilsMessengerEXT debugMessenger;
			VkSurfaceKHR surface;

			VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;


			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;



			const std::vector<const char*> validationLayers = {
				"VK_LAYER_KHRONOS_validation"
			};


			bool isComplete () {
				return graphicsFamily.has_value () && presentFamily.has_value ();
			}
		public:

			static std::unique_ptr<PhysicalDevice> create ( VkInstance instance, VkSurfaceKHR surface, VkPhysicalDeviceType deviceType );
			static PhysicalDevice create1 ( VkInstance instance, VkSurfaceKHR surface, VkPhysicalDeviceType deviceType );

			void createLogicalDevice ();
			void findQueueFamilies ();
			void querySwapChainSupport ();

			VkQueue graphicsQueue;
			VkQueue presentQueue;

			PhysicalDevice ( VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice );
			VkImageView createImageView ( VkImage image, VkFormat format );
			SwapChain createSwapChain ( GLFWwindow* window );
			std::vector<VkFramebuffer> createFramebuffers ( SwapChain swapChain, VkRenderPass renderPass, std::vector<VkImageView> attachments );
			VkFramebuffer createShadowFramebuffer ( VkRenderPass shadowRenderPass, VkImageView depthImageView, VkExtent2D shadowExtent );

			VkCommandPool createCommandPool ();
			//auto d =Device (physicalDevice, device, graphicsQueue);
			Device* createDevice ();
			VkSurfaceFormatKHR chooseSwapSurfaceFormat ( const std::vector<VkSurfaceFormatKHR>& availableFormats ) {
				for (const auto& availableFormat : availableFormats) {
					if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
						return availableFormat;
					}

					if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
						return availableFormat;
					}
				}

				return availableFormats[1];
			}

			VkPresentModeKHR chooseSwapPresentMode ( const std::vector<VkPresentModeKHR>& availablePresentModes ) {
				for (const auto& availablePresentMode : availablePresentModes) {
					if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
						return availablePresentMode;
					}
				}

				return VK_PRESENT_MODE_FIFO_KHR;
			}

			VkExtent2D chooseSwapExtent ( const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window ) {
				if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max ()) {
					return capabilities.currentExtent;
				}
				else {
					int width, height;
					glfwGetFramebufferSize ( window, &width, &height );

					VkExtent2D actualExtent = {
						static_cast<uint32_t>(width),
						static_cast<uint32_t>(height)
					};

					actualExtent.width = std::clamp ( actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width );
					actualExtent.height = std::clamp ( actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height );

					return actualExtent;
				}
			}
		};

	}


}