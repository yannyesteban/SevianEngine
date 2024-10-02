#pragma once
#include "Device.h"
#include "TextureManager.h"
#include <Vertex.h>
#include <RenderInterface.h>
#include "MeshEntity.h"


using namespace SEVIAN;
namespace VULKAN {
	class MeshManager
	{
	private:
		Device* device;
		TextureManager* textureManager;

		VkDescriptorSetLayout descriptorSetLayout;
		Pipeline pipeline;

		std::vector<BufferInfo> buffersInfo = {};
		std::vector<VkDescriptorSet> descriptorSets;

		std::vector<VulkanUBuffer> ubo;
		std::vector<VulkanUBuffer> ubo2;
		std::vector<VulkanUBuffer> light;
		std::vector<VulkanUBuffer> me;
		  
	public: 
		MeshManager ( Device* device, TextureManager* textureManager );
		std::unique_ptr<SEVIAN::Entity3D> create ( SEVIAN::Info3D info );
	};
}


