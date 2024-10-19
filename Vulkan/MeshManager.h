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

		VkDescriptorSetLayout bufDescriptorSetLayout;
		VkDescriptorSetLayout texDescriptorSetLayout;
		VkDescriptorSetLayout shadowDescriptorSetLayout;


		VkPipeline pipeline;
		VkPipeline shadowPipeline;
		VkPipelineLayout pipelineLayout;
		VkPipelineLayout shadowPipelineLayout;
		//Pipeline pipeline;

		std::vector<DSInfo> buffersInfo = {};
		std::vector<VkDescriptorSet> descriptorSets;

		std::vector<VulkanUBuffer> ubo;
		std::vector<VulkanUBuffer> ubo2;
		std::vector<VulkanUBuffer> light;
		std::vector<VulkanUBuffer> me;
		  
		Pipeline createGraphPipeline ( VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions, std::vector<VkDescriptorSetLayout> descriptorSetLayouts, std::string vertSource, std::string fragSource );
		VkPipeline createGraphPipeline ( VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions, VkPipelineLayout pipelineLayout, std::string vertSource, std::string fragSource );
		VkPipeline createGraphShadowPipeline ( VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions, VkPipelineLayout pipelineLayout, std::string vertSource, std::string fragSource );
	public: 
		MeshManager ( Device* device, TextureManager* textureManager );
		std::unique_ptr<SEVIAN::Entity3D> create ( SEVIAN::Info3D info );
	};
}


