#pragma once
#include "RenderInterface.h"
#include "Device.h"
//#include "VKElement.h"
#include "Renderer.h"



namespace SEVIAN {

	namespace VULKAN {
		class SpriteManager : public RENDERER::iSpriteManager
		{
			// Heredado vía iSpriteManager
		private:
			//VulkanInfo vulkan;
			Device* device;
			std::vector<Frame> frames;
			VkDescriptorPool descriptorPool;

			std::shared_ptr<VulkanRenderer> renderer;


			VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
			VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
			VkPipeline pipeline = VK_NULL_HANDLE;

			//std::vector<VulkanUBuffer> ubo;
			std::vector<VulkanUBuffer> ubo;

			//VertexBuffer vertexBuffer;
			//VertexBuffer indicesBuffer;
			size_t indicesSizes;

			std::vector<VkDescriptorSet> descriptorSets;
			void setDescriptorSetLayout ();
			void setPipelineLayout ();
			void setGraphPipeline ();
			VkPipeline createGraphPipeline ( VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions, VkPipelineLayout pipelineLayout, std::string vertSource, std::string fragSource );
		public:
			SpriteManager ( VulkanInfo vulkan );
			SpriteManager ( std::shared_ptr<VulkanRenderer> renderer ) :device ( renderer->device ), descriptorPool(renderer->descriptorPool), frames(renderer->frames ) { 
				setDescriptorSetLayout ();
				setPipelineLayout ();
				setGraphPipeline ();
			};
			
			std::unique_ptr<RENDERER::IRenderizable> createSprite ( SEVIAN::SpriteInfo info ) override;


			
		};
	}

}


