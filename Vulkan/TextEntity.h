#pragma once

#include <msdfgen.h>
#include <msdfgen-ext.h>
#include <algorithm>
#include <vector>
#include "Device.h"
#include "TextureManager.h"
#include <RenderInterface.h>
#include "VulkanProperty.h"
#include "TextEngine.h"
//#include "VulkanRenderer.h"
using namespace SEVIAN;

/*
struct FontInfo
{
	std::string fontPath;
	double size = 32;
	int cols = 10;
	int rows = 10;
	unsigned char firstGlyph = 32;
	unsigned char lastGlyph = 128;

	double border = 0.0;
	float range = 0.050;
	bool centered = true;
	bool edgeColoring = true;
	double angleThreshold = 3.0;
};
*/
class TextEngine
{
public:
	virtual void createAtlas ( std::string name, AtlasInfo  info ) = 0;
	virtual void setFont ( std::string name ) = 0;
	virtual void render ( std::string, float scale ) = 0;

};
namespace VULKAN {

	

	struct VertexText
	{
		glm::vec2 pos;
		glm::vec2 texCoord;
		int color = 1;

		static VkVertexInputBindingDescription getBindingDescription () {
			VkVertexInputBindingDescription bindingDescription {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof ( VertexText );
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions () {

			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions {};
			
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof ( VertexText, pos );

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[1].offset = offsetof ( VertexText, texCoord );

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R8_SINT;
			attributeDescriptions[2].offset = offsetof ( VertexText, color );

			return attributeDescriptions;

		}
	};

	enum FontRenderType
	{
		SDF,
		PSDF,
		MSDF,
		MTSDF
	};

	
	/*
	struct GlyphInfo
	{
	public:
		float width;
		float height;
		float bearingX;
		float bearingY;
		
		float u0;
		float v0;
		float u1;
		float v1;

		float advance;


	};
	*/
	class TextEntity :
		public Entity3D
	{
		
		VulkanTexture texture;
		
		std::string text;
		std::string font;
		
		Device* device;
		std::vector<Frame> frames;
		VkDescriptorPool descriptorPool;

		//Glyph character;
		//Glyph character2;
		std::shared_ptr<Entity3D> prop;
		bool play = false;
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
		std::vector<std::shared_ptr<Entity3D>> tx;

		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkPipeline pipeline = VK_NULL_HANDLE;

		VkPipelineLayout shadowPipelineLayout = VK_NULL_HANDLE;
		VkPipeline shadowPipeline = VK_NULL_HANDLE;

		
		std::map<char, AtlasGlyphInfo> Characters;

		
		Font f;
		
	

		void createImageView ( VkImage image, VkFormat format, VkImageView& imageView );

		void createTextureSampler ( VkSampler& sampler );

		VkPipeline createGraphPipeline ( VkVertexInputBindingDescription bindingDescription, std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions, VkPipelineLayout pipelineLayout, std::string vertSource, std::string fragSource );

		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
		
		
		VulkanTexture createAtlasTexture ( std::vector<uint8_t> msdfData, int atlasWidth, int atlasHeight );

		void updateUniformBuffer ( void* uniformBuffersMapped, glm::vec3 position, Camera camera, uint32_t width, uint32_t height );

	public:
		void fontInit ( AtlasInfo info );
		
		TextEntity (
			FontRenderType type,
			std::string text,
			std::string font,
			Device* device,
			std::vector<Frame> frames,
			VkDescriptorPool descriptorPool );
		
		void render ( UniformBufferObject ubo ) override;
		
		void ShadowRender ( UniformBufferObject ubo ) override;

		
		//void fontInit4 ( std::string fontPath );
		void draw ( std::string text, uint32_t currentFrame, VkCommandBuffer commandBuffer, glm::vec3 position, Camera camera, uint32_t width, uint32_t height );
		std::shared_ptr<Entity3D> init ( std::vector<VertexTextOld> vertices, std::vector<uint32_t> indices, VulkanTexture texture );
	};

}