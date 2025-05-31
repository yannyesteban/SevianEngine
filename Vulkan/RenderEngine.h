#pragma once
#include "RenderInterface.h"
#include "Device.h"
//#include "VKElement.h"
#include "Renderer.h"
#include "VulkanRendererManager.h"

namespace SEVIAN::VULKAN {
	class RenderEngine
	{
	private:
		Device* device;
		VkDescriptorPool descriptorPool;
	protected:


		std::vector<std::vector<DescriptorBinding*>> descriptorInfo;
		std::vector<PushConstantInfo*> pushConstantsInfo;

		std::unordered_map<RENDERER::DataResource, std::unique_ptr<Resource>> _resources;
	public:

		void add ( RENDERER::DataResource resourceType, uint32_t set, VkDescriptorType type, uint32_t binding, VkShaderStageFlagBits stage );
		void addPushConstant ( RENDERER::DataResource resourceType, VkShaderStageFlags stage, uint32_t size, uint32_t offset );
		VkDescriptorSetLayout createDescriptorSetLayout ( std::vector<DescriptorBinding*> info );
		std::vector<VkPushConstantRange> createPushConstantRange ( std::vector<PushConstantInfo*> pushConstantsInfo );
		VkPipelineLayout createPipelineLayout ( std::vector<VkDescriptorSetLayout> layouts, std::vector<VkPushConstantRange> pushConstantRangesVector );
		VkPipeline createGraphPipeline ( VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions, VkPipelineLayout pipelineLayout, std::string vertSource, std::string fragSource );
		void createDescriptorSets ( std::vector<FrameData>& frames, VkDescriptorSetLayout descriptorSetLayout, std::vector<DescriptorSetDataInfo> info );
	};

}
