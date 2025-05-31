#pragma once
#include <unordered_map>
#include "Vulkan.h"
#include "iRenderManager.h"
#include "Device.h"
#include "Renderer.h"


namespace SEVIAN::VULKAN {
	class ResourceData
	{

	public:
		ResourceData ( Device* device );

		
		
		std::vector<DescriptorBinding*> descriptorBindingInfo;
		std::vector<PushConstantInfo*> pushConstantsInfo;

		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkPipeline pipeline = VK_NULL_HANDLE;
		std::vector<VkDescriptorSetLayout> layouts;
		std::vector<std::vector<DescriptorBinding*>> descriptorInfo;
		

		std::unordered_map<RENDERER::DataResource, std::unique_ptr<DescriptorBinding>> resourcesData;



		void addResourceData ( RENDERER::DataResource resourceType, uint32_t set, VkDescriptorType type, uint32_t binding, VkShaderStageFlagBits stage );
		void addPushConstant ( RENDERER::DataResource resourceType, VkShaderStageFlags stage, uint32_t size, uint32_t offset );

		void doDescriptorSets ( std::shared_ptr<VKElement>& element, VkDescriptorPool descriptorPool );
		void createDescriptorSets ( std::vector<FrameData>& frames, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, std::vector<DescriptorSetDataInfo> info );
		void init ();
	private:
		Device* device;
		
		std::unordered_map<RENDERER::DataResource, std::unique_ptr<Resource>> resources;

		
		
	}; 

}