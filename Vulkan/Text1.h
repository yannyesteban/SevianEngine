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
#include "Text.h"


class TextLayout
{


private:
	Font font;
	uint32_t width;
	uint32_t height;

	uint32_t x;
	uint32_t y;

public:
	TextLayout ( Font font, uint32_t width, uint32_t height ) : width ( width ), height ( height ), x(0), y(0), font(font) { };

	std::vector<std::vector<Quad>> addText ( std::string text );
	float wordSize ( std::string );
	void createTextBox ( std::string text, uint32_t width, uint32_t height ) {

	}
};

namespace VULKAN {

	struct InstanceText
	{
		glm::vec2 pos;
		glm::vec2 size;
		glm::vec2 scale;
		glm::vec2 uv;

		static VkVertexInputBindingDescription getBindingDescription () {
			VkVertexInputBindingDescription bindingDescription {};
			bindingDescription.binding = 1;  // Binding para el Instance Buffer
			bindingDescription.stride = sizeof ( InstanceText );
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions () {
			std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions {};

			attributeDescriptions[0].binding = 1;
			attributeDescriptions[0].location = 3; // Continuación de las locations
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof ( InstanceText, pos );

			attributeDescriptions[1].binding = 1;
			attributeDescriptions[1].location = 4;
			attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[1].offset = offsetof ( InstanceText, size );

			attributeDescriptions[2].binding = 1;
			attributeDescriptions[2].location = 5;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof ( InstanceText, scale );

			attributeDescriptions[3].binding = 1;
			attributeDescriptions[3].location = 6;
			attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[3].offset = offsetof ( InstanceText, uv );

			return attributeDescriptions;
		}

	};

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

	class Text
	{
	private:
		std::string text;
		Device* device;
		TextureManager* textureManager;

		Propertys propertys;
		VulkanTexture texture;
		Font font;

		//std::shared_ptr<VulkanProperty> vulkanProp;
		std::vector<std::shared_ptr<Entity3D>> elements;
	public:

		void createGlyph ( AtlasGlyphInfo );

		void Text2 ( std::vector<std::vector<Quad>> lines );

		Text ( Device* device, Propertys propertys, VulkanTexture texture, Font font, std::string text );
		void render ( uint32_t width, uint32_t height );
		std::shared_ptr<Entity3D> init ( std::vector<VertexText> vertices, std::vector<uint32_t> indices, VulkanTexture texture );
		void updateUniformBuffer ( void* uniformBuffersMapped, glm::vec3 position, uint32_t width, uint32_t height );
		void setText ( std::string text );
		void setScale ( float scale );


	};

}