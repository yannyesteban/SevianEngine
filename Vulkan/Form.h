#pragma once
#include "VulkanProperty.h"
#include "Device.h"

namespace VULKAN {


	struct ButtonUBO
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
		

		
	};

	class Rectangle
	{
	private:
		VulkanInfo vulkan;
		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkPipeline pipeline = VK_NULL_HANDLE;

		std::vector<VulkanUBuffer> ubo;


		VertexBuffer vertexBuffer;
		VertexBuffer indicesBuffer;
		size_t indicesSizes;

		std::vector<VkDescriptorSet> descriptorSets;

	public:
		Rectangle ( VulkanInfo vulkan,
			VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE,
			VkPipelineLayout pipelineLayout = VK_NULL_HANDLE,
			VkPipeline pipeline = VK_NULL_HANDLE ) ;
		void updateUniformBuffer ( int32_t index, const UniformBufferObject& ubo );
		void render ( VkCommandBuffer commandBuffer, uint32_t frameIndex );
	};

	class Form
	{

	private:
		VulkanInfo vulkan;
		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkPipeline pipeline = VK_NULL_HANDLE;

	public:

		Form ( VulkanInfo info );

		void setDescriptorSetLayout ();
		void setPipelineLayout ();

		void setGraphPipeline ();
		Rectangle createRect ( float x, float y, float width, float height );

		VkPipeline createGraphPipeline ( VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions, VkPipelineLayout pipelineLayout, std::string vertSource, std::string fragSource );

	};

}


