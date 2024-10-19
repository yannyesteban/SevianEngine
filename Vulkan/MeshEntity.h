#pragma once
#include "Device.h"
#include "TextureManager.h"
#include "RenderInterface.h"
//#include "VulkanRenderer.h"
using namespace SEVIAN;

VkVertexInputBindingDescription getBindingDescriptionGeneric ();
std::vector<VkVertexInputAttributeDescription> getAttributeDescriptionsGeneric ();

namespace VULKAN {
    class MeshEntity : public Entity3D {
    
    private:
		Device* device;
		TextureManager* textureManager;
		Info3D info;

		Propertys propertys;

		VertexBuffer vertex;
		VertexBuffer indices;
		size_t indicesSizes;

		VkDescriptorSetLayout bufDescriptorSetLayout = VK_NULL_HANDLE;
		VkDescriptorSetLayout texDescriptorSetLayout = VK_NULL_HANDLE;
		VkDescriptorSetLayout shadowDescriptorSetLayout = VK_NULL_HANDLE;

		std::vector<VkDescriptorSet> bufDescriptorSets;
        std::vector<VkDescriptorSet> texDescriptorSets;
		std::vector<VkDescriptorSet> shadowDescriptorSets;

		std::vector<VulkanUBuffer> ubo;
		std::vector<VulkanUBuffer> ubo1;

		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkPipeline pipeline = VK_NULL_HANDLE;

		VkPipelineLayout shadowPipelineLayout = VK_NULL_HANDLE;
		VkPipeline shadowPipeline = VK_NULL_HANDLE;

		

    public: 
		MeshEntity ( Device* device, TextureManager* textureManager, Info3D info, VkPipelineLayout pipelineLayout, VkPipeline pipeline, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSetLayout texDescriptorSetLayout );
		MeshEntity ( Device* device, TextureManager* textureManager, Info3D info, Propertys propertys );
        void render ( UniformBufferObject ubo ) override;
		void ShadowRender ( UniformBufferObject ubo ) override;

       
       
    };

    
}


