#pragma once
#include <Vertex.h>
#include "Device.h"
#include "TextureManager.h"
#include "RenderInterface.h"

static VkVertexInputBindingDescription getBindingDescription24 ();
static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions24 ();

using namespace SEVIAN;

namespace VULKAN {
	class VulkanEntity: public Entity3D
	{
	private:
		Device* device;
		TextureManager * textureManager;
		Info3D info;
		

		VertexBuffer vertex;
		VertexBuffer indices;

		VkPipeline pipeline = VK_NULL_HANDLE;
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

		std::vector<VkDescriptorSet> descriptorSets;

		size_t indicesSizes;

		VkBuffer vertexBuffer = VK_NULL_HANDLE;
		VkBuffer indexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
		VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;
		std::vector<VulkanUBuffer> ubo;
		std::vector<VulkanUBuffer> ubo2;
		std::vector<VulkanUBuffer> light;
		std::vector<VulkanUBuffer> me;

		Pipeline pipe;

	public:
		VulkanEntity ( Device* device, TextureManager* textureManager, Info3D info );
		void render ( UniformBufferObject ubo ) override;
		void ShadowRender ( UniformBufferObject ubo ) override;
	};


}
