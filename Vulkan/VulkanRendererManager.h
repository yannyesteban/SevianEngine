#pragma once
#include "Vulkan.h"
#include "iRenderManager.h"
#include "Device.h"
#include "Renderer.h"

namespace SEVIAN::VULKAN {
	class VulkanRendererManager: public RENDERER::ObjectManager
	{
	private:
		Device* device;
		std::vector<Frame> frames;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSetLayout> layouts;
		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkPipeline pipeline = VK_NULL_HANDLE;

		std::vector<std::vector<DescriptorBinding*>> descriptorInfo;
		std::unordered_map<RENDERER::DataResource, std::unique_ptr<Resource>> _resources;
		std::vector<DescriptorBinding*> descriptorBindingInfo;
		std::vector<PushConstantInfo*> pushConstantsInfo;

	protected:
		

		void add ( RENDERER::DataResource resourceType, uint32_t set, VkDescriptorType type, uint32_t binding, VkShaderStageFlagBits stage );
		void addPushConstant ( RENDERER::DataResource resourceType, VkShaderStageFlags stage, uint32_t size, uint32_t offset );
		VkDescriptorSetLayout createDescriptorSetLayout ( std::vector<DescriptorBinding*> info );
		std::vector<VkPushConstantRange> createPushConstantRange ( std::vector<PushConstantInfo*> pushConstantsInfo );
		void createUniformBuffer ( RENDERER::DataResource resourceId, std::vector<FrameData>& frames, VkDeviceSize bufferSize );

		VkPipelineLayout createPipelineLayout ( std::vector<VkDescriptorSetLayout> layouts, std::vector<VkPushConstantRange> pushConstantRangesVector );
		/*
		virtual VkPipeline createGraphPipeline ( VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions, VkPipelineLayout pipelineLayout, std::string vertSource, std::string fragSource ) = 0;*/
	public:
	};

}