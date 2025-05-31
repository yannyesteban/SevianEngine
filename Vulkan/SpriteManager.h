#pragma once
#include "RenderInterface.h"
#include "Device.h"
//#include "VKElement.h"
#include "Renderer.h"
#include "VulkanRendererManager.h"
#include "ResourceData.h"

namespace SEVIAN {

	namespace VULKAN {
		
		class SpriteManager : public RENDERER::iSpriteManager
		{
			
		private:

			Device* device;
			std::vector<Frame> frames;
			VkDescriptorPool descriptorPool;
			std::shared_ptr<VulkanRenderer> renderer;
			VkPipeline pipeline = VK_NULL_HANDLE;

			VkPipeline createGraphPipeline ( VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions, VkPipelineLayout pipelineLayout, std::string vertSource, std::string fragSource );

			std::shared_ptr<ResourceData> resourceData;

			void createUniformBuffer ( RENDERER::DataResource resourceId, std::vector<FrameData>& frames, VkDeviceSize bufferSize );
		
		public:
			SpriteManager ( std::shared_ptr<VulkanRenderer> renderer );
			
			std::unique_ptr<RENDERER::IRenderizable> createSprite ( SEVIAN::SpriteInfo info ) override;
			
		};
	}

}


