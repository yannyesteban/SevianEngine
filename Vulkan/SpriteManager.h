#pragma once
#include "RenderInterface.h"
#include "Device.h"
/*
namespace SEVIAN {

	namespace VULKAN {
		class SpriteManager : public RENDERER::iSpriteManager
		{
			// Heredado vía iSpriteManager
		private:
			VULKAN::VulkanInfo vulkan;
			VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
			VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
			VkPipeline pipeline = VK_NULL_HANDLE;

			std::vector<VulkanUBuffer> ubo;


			VertexBuffer vertexBuffer;
			VertexBuffer indicesBuffer;
			size_t indicesSizes;

			std::vector<VkDescriptorSet> descriptorSets;
		public:
			SpriteManager ( VulkanInfo vulkan );
			void setDescriptorSetLayout ();
			void setPipelineLayout ();
			void setGraphPipeline ();
			VkPipeline createGraphPipeline ( VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions, VkPipelineLayout pipelineLayout, std::string vertSource, std::string fragSource );
			RENDERER::iObject createSprite ( SEVIAN::SpriteInfo info ) override;
		};
	}
}


*/