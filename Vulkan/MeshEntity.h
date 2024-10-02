#pragma once
#include "Device.h"
#include "TextureManager.h"
#include "RenderInterface.h"
//#include "VulkanRenderer.h"
using namespace SEVIAN;

static VkVertexInputBindingDescription getBindingDescriptionGeneric ();
static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptionsGeneric ();

namespace VULKAN {
    class MeshEntity : public Entity3D {
    
    private:
		Device* device;
		TextureManager* textureManager;
		Info3D info;


		VertexBuffer vertex;
		VertexBuffer indices;

		//VkPipeline pipeline = VK_NULL_HANDLE;
		//VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<VkDescriptorSet> descriptorSets;
        std::vector<VkDescriptorSet> texDescriptorSets;

		size_t indicesSizes;

		VkBuffer vertexBuffer = VK_NULL_HANDLE;
		VkBuffer indexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
		VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;
		

		Pipeline pipeline;

		MeshEntity ( Device* device, TextureManager* textureManager, Info3D info, std::vector<VkDescriptorSet> _descriptorSetLayout, Pipeline _pipeline, VkDescriptorSetLayout descriptorSetLayout );


    public: 
        MeshEntity ( Device* device, TextureManager* textureManager, Info3D info );
        MeshEntity ( Device* device, TextureManager* textureManager, VkDescriptorSetLayout descriptorSetLayout, std::vector<VkDescriptorSet> descriptorSet, Pipeline pipeline, Info3D info );
        void render ( UniformBufferObject ubo ) override;

        std::vector<VulkanUBuffer> ubo;
        std::vector<VulkanUBuffer> ubo2;
        std::vector<VulkanUBuffer> light;
        std::vector<VulkanUBuffer> me;
    };

    
}


