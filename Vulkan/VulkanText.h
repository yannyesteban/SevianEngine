#pragma once
#include "Vulkan.h"
#include "Vertex.h"
#include "VulkanProperty.h"
#include "Device.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <math.h>
#include <map>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

#define WIDTH   200
#define HEIGHT  100




namespace VULKAN {

	

	static VkVertexInputBindingDescription getBindingDescription2 () {
		VkVertexInputBindingDescription bindingDescription {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof ( SEVIAN::VertexText );
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions2 () {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions ( 2, {} );
		/*    glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 color;*/
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof ( SEVIAN::VertexText, pos );

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[1].offset = offsetof ( SEVIAN::VertexText, texCoord );



		return attributeDescriptions;
	}

	struct Character
	{
		VkImage        textureImage;
		VkDeviceMemory textureImageMemory;
		VkImageView    textureImageView;
		VkSampler      textureSampler;
		int            width;
		int            height;
		int            bearingX;
		int            bearingY;
		long           advance;
	};



	class VulkanText
	{
	private:
		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkPipeline pipeline = VK_NULL_HANDLE;
		VkPipeline createGraphPipeline ( VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions, VkPipelineLayout pipelineLayout, std::string vertSource, std::string fragSource );

	public:
		
		FT_Library  library;
		FT_Face     face;
		std::map<char, Character> Characters;

		std::vector<Frame> frames;
		VkDescriptorPool descriptorPool;

		std::vector<std::shared_ptr<Entity3D>> tx;
		bool play = false;

		Device* device;
		void initialize ();
		void start ();
		void draw ( std::string text, uint32_t currentFrame, VkCommandBuffer commandBuffer, glm::vec3 position,
			Camera camera, uint32_t width, uint32_t height );
		void createTextureImage ( FT_Face face, char c, Character& character );
		void createImageView ( VkImage image, VkFormat format, VkImageView& imageView );
		void createTextureSampler ( VkSampler& sampler );

		std::shared_ptr<Entity3D> init ( std::vector<VertexText> vertices, std::vector<uint32_t> indices, /*VulkanTexture*/ Character texture);
		Pipeline createGraphicsPipeline3 ( VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions, VkDescriptorSetLayout& descriptorSetLayout );




	};
}



