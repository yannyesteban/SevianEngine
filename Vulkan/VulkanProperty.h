#pragma once
#include "RenderInterface.h"
#include "Device.h"

namespace SEVIAN {
	class VulkanProperty : public PropertyRender
	{
	public:
		
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
		std::vector<VulkanUBuffer> buffers;
		
		

		

	};

}



