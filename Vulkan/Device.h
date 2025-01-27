#pragma once


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>


#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <array>

#include <set>

#include "Vulkan.h"
//#include "AppTypes.h"
//#include "Frame.h"
//#include "db.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

static int times = 0;
void db ( std::string message );


namespace VULKAN {
	class Device
	{
	public:
		const int MAX_FRAMES_IN_FLIGHT = 2;
		VkDevice device;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkImageView textureImageView;

		VulkanDepthResources depthResource;

		Device ( VkPhysicalDevice pPhysicalDevice, VkDevice pDevice, VkQueue pGraphicsQueue );
		VkDescriptorSetLayout createDescriptorSetLayout ();
		VkDescriptorSetLayout createDescriptorSetLayout ( std::vector <DSLInfo> info );

		VkDescriptorPool createDescriptorPool ();
		void createUniformBuffers ( Frame& frame, VkDeviceSize bufferSize );
		std::vector<VulkanUBuffer> createUniformBuffer ( std::vector<Frame>& frames, VkDeviceSize bufferSize );
		std::vector<VulkanUBuffer> createUniformBuffer ( VkDeviceSize bufferSize );
		void createCommandBuffers ( std::vector<Frame>& frames, VkCommandPool commandPool );
		void createSyncObjects ( Frame& frames );

		std::vector<VkDescriptorSet> createDescriptorSets ( std::vector<VulkanUBuffer>& buffers, VkImageView textureImageView, VkSampler textureSampler, VkDeviceSize range );
		std::vector<VkDescriptorSet> createDescriptorSets ( std::vector<Frame>& frames, VkDescriptorPool descriptorPool, VkImageView textureImageView, VkSampler textureSampler, VkDeviceSize range );
		std::vector<VkDescriptorSet> createDescriptorSets ( std::vector <DSInfo> info );
		std::vector<VkDescriptorSet> createDescriptorSets ( VkDescriptorSetLayout descriptorSetLayout, std::vector <DSInfo> info );

		VkFormat findSupportedFormat ( const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features );
		VkFormat findDepthFormat ();
		VkFormat findShadowMapFormat ();

		VulkanDepthResources createDepthResources ( VkExtent2D extent );


		VkPipelineLayout createPipelineLayout ( std::vector<VkDescriptorSetLayout> descriptorSetLayouts );

		VkPipeline createGraphicsPipeline ( VkVertexInputBindingDescription bindingDescription,
			std::vector < VkVertexInputAttributeDescription> attributeDescriptions, VkDescriptorSetLayout& descriptorSetLayout );

		/*
		Device(VkPhysicalDevice pPhysicalDevice, VkDevice pDevice, VkQueue pGraphicsQueue) {
			device = pDevice;
			physicalDevice = pPhysicalDevice;
			graphicsQueue = pGraphicsQueue;
		}
		*/
		//VulkanTexture createTexture ( const char* src );
		std::unique_ptr<VulkanTexture> createTexture ( const char* src );

		void createCommandPool ( QueueFamilyIndices queueFamilyIndices ) {
			//QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

			VkCommandPoolCreateInfo poolInfo {};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value ();

			if (vkCreateCommandPool ( device, &poolInfo, nullptr, &commandPool ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to create graphics command pool!" );
			}
		}

		VkSampler createTextureSampler () {
			VkSampler textureSampler;

			VkPhysicalDeviceProperties properties {};
			vkGetPhysicalDeviceProperties ( physicalDevice, &properties );

			VkSamplerCreateInfo samplerInfo {};
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.anisotropyEnable = VK_TRUE;
			samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
			samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			samplerInfo.unnormalizedCoordinates = VK_FALSE;
			samplerInfo.compareEnable = VK_FALSE;
			samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

			if (vkCreateSampler ( device, &samplerInfo, nullptr, &textureSampler ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to create texture sampler!" );
			}

			return textureSampler;
		}

		VkSampler createShadowMapSampler () {
			
			VkSamplerCreateInfo samplerInfo {};
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInfo.anisotropyEnable = VK_FALSE;
			samplerInfo.maxAnisotropy = 1.0f;
			samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			samplerInfo.unnormalizedCoordinates = VK_FALSE;
			samplerInfo.compareEnable = VK_TRUE;
			samplerInfo.compareOp = VK_COMPARE_OP_LESS;
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

			VkSampler textureSampler;
			if (vkCreateSampler ( device, &samplerInfo, nullptr, &textureSampler ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to create texture sampler!" );
			}

			return textureSampler;
		}

		VertexBuffer createVertexBuffer () {

			VertexBuffer buffer;

			//VkBuffer vertexBuffer;
			//VkDeviceMemory vertexBufferMemory;


			const std::vector<Vertex2> vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
			};

			VkDeviceSize bufferSize = sizeof ( vertices[0] ) * vertices.size ();

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			createBuffer ( bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory );

			void* data;
			vkMapMemory ( device, stagingBufferMemory, 0, bufferSize, 0, &data );
			memcpy ( data, vertices.data (), (size_t) bufferSize );
			vkUnmapMemory ( device, stagingBufferMemory );

			createBuffer ( bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer.buffer, buffer.memory );

			copyBuffer ( stagingBuffer, buffer.buffer, bufferSize );

			vkDestroyBuffer ( device, stagingBuffer, nullptr );
			vkFreeMemory ( device, stagingBufferMemory, nullptr );

			return buffer;
		}

		VertexBuffer createDataBuffer ( void* points, VkDeviceSize bufferSize, VkBufferUsageFlagBits f ) {

			VertexBuffer buffer;

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			createBuffer ( bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory );

			void* data;
			vkMapMemory ( device, stagingBufferMemory, 0, bufferSize, 0, &data );
			memcpy ( data, points, (size_t) bufferSize );
			vkUnmapMemory ( device, stagingBufferMemory );

			createBuffer ( bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | f, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer.buffer, buffer.memory );

			copyBuffer ( stagingBuffer, buffer.buffer, bufferSize );

			vkDestroyBuffer ( device, stagingBuffer, nullptr );
			vkFreeMemory ( device, stagingBufferMemory, nullptr );

			return buffer;
		}


		DataPipeline createGraphicsPipeline1 ( VkDescriptorSetLayout descriptorSetLayout, VkRenderPass renderPass, VkShaderModule vertShaderModule, VkShaderModule fragShaderModule ) {
			DataPipeline pipeline;
			/*auto vertShaderCode = readFile("shaders/vert.spv");
			auto fragShaderCode = readFile("shaders/frag.spv");

			VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
			VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);
			*/
			VkPipelineShaderStageCreateInfo vertShaderStageInfo {};
			vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vertShaderStageInfo.module = vertShaderModule;
			vertShaderStageInfo.pName = "main";

			VkPipelineShaderStageCreateInfo fragShaderStageInfo {};
			fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragShaderStageInfo.module = fragShaderModule;
			fragShaderStageInfo.pName = "main";

			VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

			VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

			VkVertexInputBindingDescription bindingDescription {};
			bindingDescription.binding = 0;
			//bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;


			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions {};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			//attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			//attributeDescriptions[1].offset = offsetof(Vertex, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			//attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

			//auto attributeDescriptions = attributeDescriptions;//Vertex::getAttributeDescriptions();

			vertexInputInfo.vertexBindingDescriptionCount = 1;
			vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size ());
			vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
			vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data ();

			VkPipelineInputAssemblyStateCreateInfo inputAssembly {};
			inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssembly.primitiveRestartEnable = VK_FALSE;

			VkPipelineViewportStateCreateInfo viewportState {};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.scissorCount = 1;

			VkPipelineRasterizationStateCreateInfo rasterizer {};
			rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable = VK_FALSE;
			rasterizer.rasterizerDiscardEnable = VK_FALSE;
			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizer.lineWidth = 1.0f;
			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			rasterizer.depthBiasEnable = VK_FALSE;

			VkPipelineMultisampleStateCreateInfo multisampling {};
			multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable = VK_FALSE;
			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

			VkPipelineColorBlendAttachmentState colorBlendAttachment {};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_FALSE;

			VkPipelineColorBlendStateCreateInfo colorBlending {};
			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY;
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &colorBlendAttachment;
			colorBlending.blendConstants[0] = 0.0f;
			colorBlending.blendConstants[1] = 0.0f;
			colorBlending.blendConstants[2] = 0.0f;
			colorBlending.blendConstants[3] = 0.0f;

			std::vector<VkDynamicState> dynamicStates = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
			};
			VkPipelineDynamicStateCreateInfo dynamicState {};
			dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size ());
			dynamicState.pDynamicStates = dynamicStates.data ();

			VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 1;
			pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

			if (vkCreatePipelineLayout ( device, &pipelineLayoutInfo, nullptr, &pipeline.layout ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to create pipeline layout!" );
			}

			VkGraphicsPipelineCreateInfo pipelineInfo {};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount = 2;
			pipelineInfo.pStages = shaderStages;
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizer;
			pipelineInfo.pMultisampleState = &multisampling;
			pipelineInfo.pColorBlendState = &colorBlending;
			pipelineInfo.pDynamicState = &dynamicState;
			pipelineInfo.layout = pipeline.layout;
			pipelineInfo.renderPass = renderPass;
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

			if (vkCreateGraphicsPipelines ( device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline.pipeline ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to create graphics pipeline!" );
			}

			vkDestroyShaderModule ( device, fragShaderModule, nullptr );
			vkDestroyShaderModule ( device, vertShaderModule, nullptr );
		}
		void test () {
			db ( "is a device test" );
		}

	public:
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		uint32_t currentFrame = 0;
		VkSwapchainKHR swapChain;
		bool framebufferResized = false;
		std::vector<VkCommandBuffer> commandBuffers;

		VkCommandPool commandPool;
		//VkImageView textureImageView;
		VkImage textureImage;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkDeviceMemory textureImageMemory;

		VkRenderPass renderPass;
		VkRenderPass shadowRenderPass;
		VkDescriptorSetLayout descriptorSetLayout;
		VkPipelineLayout pipelineLayout;
		VkPipelineLayout pipelineLayout2;
		VkPipeline graphicsPipeline;
		VkPipeline graphicsPipeline2;
		VkSampler shadowMapSampler;

		VertexBuffer buffer;
		VertexBuffer iBuffer;
		VertexBuffer buffer2;

		VkExtent2D swapChainExtent;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		std::vector<VkDescriptorSet> descriptorSets;
		VkFramebuffer shadowFrameBuffer;
		void createTextureImage ();

		void createTextureImageView () {
			textureImageView = createImageView ( textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT );
		}
		void createImage ( uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory ) {
			VkImageCreateInfo imageInfo {};
			imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			imageInfo.format = format;
			imageInfo.extent = { width, height, 1 };
			//imageInfo.extent.width = width;
			//imageInfo.extent.height = height;
			//imageInfo.extent.depth = 1;
			imageInfo.mipLevels = 1;
			imageInfo.arrayLayers = 1;
			
			imageInfo.tiling = tiling;
			imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageInfo.usage = usage;
			imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			//imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageInfo.flags = 0; // Optional

			if (vkCreateImage ( device, &imageInfo, nullptr, &image ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to create image!" );
			}

			VkMemoryRequirements memRequirements;
			vkGetImageMemoryRequirements ( device, image, &memRequirements );

			VkMemoryAllocateInfo allocInfo {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = findMemoryType ( memRequirements.memoryTypeBits, properties );
			
			if (vkAllocateMemory ( device, &allocInfo, nullptr, &imageMemory ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to allocate image memory!" );
			}

			vkBindImageMemory ( device, image, imageMemory, 0 );
		}
		VkImageView createImageView ( VkImage image, VkFormat format, VkImageAspectFlags aspectFlags ) {
			VkImageViewCreateInfo viewInfo {};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = image;
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = format;
			viewInfo.subresourceRange = {};
			viewInfo.subresourceRange.aspectMask = aspectFlags;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;// VK_REMAINING_MIP_LEVELS;// 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;// VK_REMAINING_ARRAY_LAYERS;// 1;

			// Stencil aspect should only be set on depth + stencil formats (VK_FORMAT_D16_UNORM_S8_UINT..VK_FORMAT_D32_SFLOAT_S8_UINT
			if (format >= VK_FORMAT_D16_UNORM_S8_UINT) {
				viewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}

			VkImageView imageView;
			if (vkCreateImageView ( device, &viewInfo, nullptr, &imageView ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to create texture image view!" );
			}

			return imageView;
		}
		void createBuffer ( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory );

		uint32_t findMemoryType ( uint32_t typeFilter, VkMemoryPropertyFlags properties ) {
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties ( physicalDevice, &memProperties );

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}

			throw std::runtime_error ( "failed to find suitable memory type!" );
		}

		void transitionImageLayout ( VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout ) {
			VkCommandBuffer commandBuffer = beginSingleTimeCommands ();

			VkImageMemoryBarrier barrier {};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = oldLayout;
			barrier.newLayout = newLayout;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = image;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;

			VkPipelineStageFlags sourceStage;
			VkPipelineStageFlags destinationStage;

			if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
				barrier.srcAccessMask = 0;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}
			else {
				throw std::invalid_argument ( "unsupported layout transition!" );
			}

			vkCmdPipelineBarrier (
				commandBuffer,
				sourceStage, destinationStage,
				0,
				0, nullptr,
				0, nullptr,
				1, &barrier
			);

			endSingleTimeCommands ( commandBuffer );
		}

		VkCommandBuffer beginSingleTimeCommands () {
			VkCommandBufferAllocateInfo allocInfo {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = commandPool;
			allocInfo.commandBufferCount = 1;

			VkCommandBuffer commandBuffer;



			vkAllocateCommandBuffers ( device, &allocInfo, &commandBuffer );


			//std::cout << "helooo   \n";

			VkCommandBufferBeginInfo beginInfo {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			vkBeginCommandBuffer ( commandBuffer, &beginInfo );

			return commandBuffer;
		}

		void endSingleTimeCommands ( VkCommandBuffer commandBuffer ) {
			vkEndCommandBuffer ( commandBuffer );

			VkSubmitInfo submitInfo {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			vkQueueSubmit ( graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE );
			vkQueueWaitIdle ( graphicsQueue );

			vkFreeCommandBuffers ( device, commandPool, 1, &commandBuffer );
		}

		void copyBufferToImage ( VkBuffer buffer, VkImage image, uint32_t width, uint32_t height ) {
			VkCommandBuffer commandBuffer = beginSingleTimeCommands ();

			VkBufferImageCopy region {};
			region.bufferOffset = 0;
			region.bufferRowLength = 0;
			region.bufferImageHeight = 0;
			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel = 0;
			region.imageSubresource.baseArrayLayer = 0;
			region.imageSubresource.layerCount = 1;
			region.imageOffset = { 0, 0, 0 };
			region.imageExtent = {
				width,
				height,
				1
			};

			vkCmdCopyBufferToImage ( commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region );

			endSingleTimeCommands ( commandBuffer );
		}


		void copyBuffer ( VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size ) {
			VkCommandBuffer commandBuffer = beginSingleTimeCommands ();

			VkBufferCopy copyRegion {};
			copyRegion.size = size;
			vkCmdCopyBuffer ( commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion );

			endSingleTimeCommands ( commandBuffer );
		}

		VkDescriptorSetLayout createDescriptorSetLayout1 ( std::vector<TypeLayout> types ) {
			VkDescriptorSetLayout descriptorSetLayout;

			std::vector<VkDescriptorSetLayoutBinding> bindings;

			uint32_t position = 0;
			for (auto type : types) {
				VkDescriptorSetLayoutBinding layoutBinding {};
				layoutBinding.binding = position;
				layoutBinding.descriptorCount = 1;
				layoutBinding.pImmutableSamplers = nullptr;
				switch (type) {
				case UNIFORM_BUFFER:
					layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
					break;
				case IMAGE_SAMPLER:
					layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
					break;
				}
				bindings.push_back ( layoutBinding );
			}

			VkDescriptorSetLayoutCreateInfo layoutInfo {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size ());
			layoutInfo.pBindings = bindings.data ();

			if (vkCreateDescriptorSetLayout ( device, &layoutInfo, nullptr, &descriptorSetLayout ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to create descriptor set layout!" );
			}

			return descriptorSetLayout;
		}

		VkRenderPass createRenderPass ( VkFormat swapChainImageFormat ) {
			
			VkAttachmentDescription colorAttachment {};
			colorAttachment.format = swapChainImageFormat;
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentDescription depthAttachment {};
			depthAttachment.format = findDepthFormat ();
			depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


			VkAttachmentReference colorAttachmentRef {};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkAttachmentReference depthAttachmentRef {};
			depthAttachmentRef.attachment = 1;
			depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;



			VkSubpassDescription subpass {};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;
			subpass.pDepthStencilAttachment = &depthAttachmentRef;
			/* extras */

			subpass.inputAttachmentCount = 0;
			subpass.pInputAttachments = nullptr;
			subpass.preserveAttachmentCount = 0;
			subpass.pPreserveAttachments = nullptr;
			subpass.pResolveAttachments = nullptr;


			VkSubpassDependency dependency {};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;


			std::array<VkSubpassDependency, 2> dependencies;

			dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
			dependencies[0].dstSubpass = 0;
			dependencies[0].srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			dependencies[0].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			dependencies[0].dependencyFlags = 0;

			dependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
			dependencies[1].dstSubpass = 0;
			dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependencies[1].srcAccessMask = 0;
			dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
			dependencies[1].dependencyFlags = 0;


			std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

			VkRenderPassCreateInfo renderPassInfo {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			//renderPassInfo.attachmentCount = 1;
			//renderPassInfo.pAttachments = &colorAttachment;
			renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size ());
			renderPassInfo.pAttachments = attachments.data ();
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;
			
			renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size ());
			renderPassInfo.pDependencies = dependencies.data ();

			//renderPassInfo.dependencyCount = 1;
			//renderPassInfo.pDependencies = &dependency;
			VkRenderPass renderPass;
			if (vkCreateRenderPass ( device, &renderPassInfo, nullptr, &renderPass ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to create render pass!" );
			}

			return renderPass;
		}

		VkRenderPass createShadowRenderPass ( ) {
			VkRenderPass renderPass;
			

			VkAttachmentDescription depthAttachment {};
			depthAttachment.format = findShadowMapFormat (); //VK_FORMAT_D16_UNORM;// VK_FORMAT_D32_SFLOAT;// VK_FORMAT_D16_UNORM;//findDepthFormat ();
			depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			
			depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			
			depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

			VkAttachmentReference depthAttachmentRef {};
			depthAttachmentRef.attachment = 0;
			depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;



			VkSubpassDescription subpass {};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 0;
			subpass.pDepthStencilAttachment = &depthAttachmentRef;


			/*VkSubpassDependency dependency { };
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;*/

			//std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

			/*VkSubpassDependency dependency = { };
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;  // Depende de operaciones externas (fuera del render pass)
			dependency.dstSubpass = 0;  // Depende del subpase 0 (el shadow pass)

			dependency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			dependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;  // Etapa para las pruebas de profundidad

			dependency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;  // Lectura desde shaders en el siguiente render pass
			dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;  // Escritura en el depth attachment
			*/

			/*VkSubpassDependency dependency = { };
			dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;  // Fuera del render pass (inicial)
			dependency.dstSubpass = 0;  // Nuestro subpass
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			dependency.dependencyFlags = 0;*/


			std::array<VkSubpassDependency, 2> dependencies;

			dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
			dependencies[0].dstSubpass = 0;
			dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			dependencies[1].srcSubpass = 0;
			dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
			dependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			dependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			VkRenderPassCreateInfo renderPassInfo {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 1;
			renderPassInfo.pAttachments = &depthAttachment;
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;
			renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size ()); //1;
			renderPassInfo.pDependencies = dependencies.data (); //&dependency;

			if (vkCreateRenderPass ( device, &renderPassInfo, nullptr, &renderPass ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to create render pass!" );
			}

			return renderPass;
		}

		void recordCommandBuffer ( VkCommandBuffer commandBuffer, uint32_t imageIndex ) {
			const std::vector<uint16_t> indices = {
				0, 1, 2, 2, 3, 0
			};

			VkCommandBufferBeginInfo beginInfo {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer ( commandBuffer, &beginInfo ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to begin recording command buffer!" );
			}

			VkRenderPassBeginInfo renderPassInfo {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass;
			renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = swapChainExtent;

			VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass ( commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE );

			vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline );

			VkViewport viewport {};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float) swapChainExtent.width;
			viewport.height = (float) swapChainExtent.height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			vkCmdSetViewport ( commandBuffer, 0, 1, &viewport );

			VkRect2D scissor {};
			scissor.offset = { 0, 0 };
			scissor.extent = swapChainExtent;
			vkCmdSetScissor ( commandBuffer, 0, 1, &scissor );

			VkBuffer vertexBuffers[] = { buffer.buffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );

			vkCmdBindIndexBuffer ( commandBuffer, iBuffer.buffer, 0, VK_INDEX_TYPE_UINT16 );

			vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr );

			vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(indices.size ()), 1, 0, 0, 0 );

			VkRect2D scissor2 {};
			VkExtent2D extend2 = { swapChainExtent.width * 0.5,swapChainExtent.height * 0.5 };
			scissor2.offset = { 0, 0 };
			scissor2.extent = extend2;
			vkCmdSetScissor ( commandBuffer, 0, 1, &scissor2 );

			vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline2 );

			VkBuffer vertexBuffers2[] = { buffer2.buffer };
			VkDeviceSize offsets2[] = { 0 };
			vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers2, offsets2 );

			vkCmdBindIndexBuffer ( commandBuffer, iBuffer.buffer, 0, VK_INDEX_TYPE_UINT16 );

			vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout2, 0, 1, &descriptorSets[currentFrame], 0, nullptr );

			vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(indices.size ()), 1, 0, 0, 0 );

			vkCmdEndRenderPass ( commandBuffer );

			if (vkEndCommandBuffer ( commandBuffer ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to record command buffer!" );
			}
		}
		void draw () {

			vkWaitForFences ( device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX );

			uint32_t imageIndex;
			VkResult result = vkAcquireNextImageKHR ( device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex );

			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				//recreateSwapChain();
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
				throw std::runtime_error ( "failed to acquire swap chain image!" );
			}

			//updateUniformBuffer(currentFrame);

			vkResetFences ( device, 1, &inFlightFences[currentFrame] );

			vkResetCommandBuffer ( commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0 );
			recordCommandBuffer ( commandBuffers[currentFrame], imageIndex );

			VkSubmitInfo submitInfo {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;

			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

			VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			if (vkQueueSubmit ( graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame] ) != VK_SUCCESS) {
				throw std::runtime_error ( "failed to submit draw command buffer!" );
			}

			VkPresentInfoKHR presentInfo {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;

			VkSwapchainKHR swapChains[] = { swapChain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;

			presentInfo.pImageIndices = &imageIndex;

			result = vkQueuePresentKHR ( presentQueue, &presentInfo );

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
				framebufferResized = false;
				//recreateSwapChain();
			}
			else if (result != VK_SUCCESS) {
				throw std::runtime_error ( "failed to present swap chain image!" );
			}

			currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

		}
		std::vector<char> readFile ( const std::string& filename );
		VkShaderModule createShaderModule ( const std::vector<char>& code );
		bool hasStencilComponent ( VkFormat format );

		Pipeline createGraphPipeline ( VkVertexInputBindingDescription bindingDescription,
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions,
			VkDescriptorSetLayout& descriptorSetLayout, std::string vertSource, std::string fragSource );

		Pipeline createGraphPipeline ( VkVertexInputBindingDescription bindingDescription,
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions,
			std::vector <VkDescriptorSetLayout> descriptorSetLayouts, std::string vertSource, std::string fragSource );

		std::vector<Frame> frames;
		uint32_t currentIndex ();
		Frame getFrame ();
		Frame nextFrame ();
	private:
		VkDescriptorPool descriptorPool;

	};
}




