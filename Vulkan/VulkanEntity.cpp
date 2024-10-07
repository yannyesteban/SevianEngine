#include "VulkanEntity.h"

static VkVertexInputBindingDescription getBindingDescription24 () {
	VkVertexInputBindingDescription bindingDescription {};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof ( SEVIAN::Vertex );
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}
static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions24 () {
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions ( 4, {} );
	/*    glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 color;*/
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof ( SEVIAN::Vertex, position );

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof ( SEVIAN::Vertex, normal );

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[2].offset = offsetof ( SEVIAN::Vertex, texCoords );

	attributeDescriptions[3].binding = 0;
	attributeDescriptions[3].location = 3;
	attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[3].offset = offsetof ( SEVIAN::Vertex, color );

	return attributeDescriptions;
}
using namespace SEVIAN;
namespace VULKAN {
	VulkanEntity::VulkanEntity ( Device* device, TextureManager* textureManager, Info3D info ) : device ( device ), textureManager ( textureManager ), info ( info ) {
		textureManager->add ( info.texture, info.path );
		auto texture = textureManager->get ( info.texture );


		std::vector<VulkanUBuffer> ubo = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );
		std::vector<VulkanUBuffer> ubo2 = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );
		std::vector<VulkanUBuffer> lightUBO = device->createUniformBuffer ( device->frames, sizeof ( MeUBO ) );
		std::vector<VulkanUBuffer> meUBO = device->createUniformBuffer ( device->frames, sizeof ( MeUBO ) );


		std::vector<DSInfo> buffersInfo;

		buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ubo, sizeof ( UniformBufferObject ), VK_NULL_HANDLE, VK_NULL_HANDLE, 0 } );
		//buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ubo2, sizeof ( UniformBufferObject ),  VK_NULL_HANDLE, VK_NULL_HANDLE, 1 } );
		buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, lightUBO, sizeof ( LightUBO ),  VK_NULL_HANDLE, VK_NULL_HANDLE, 1 } );
		//
		buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ubo, sizeof ( UniformBufferObject ), texture->imageView, texture->sampler, 2 } );
		buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, meUBO, sizeof ( MeUBO ),  VK_NULL_HANDLE, VK_NULL_HANDLE, 3 } );


		descriptorSets = device->createDescriptorSets ( buffersInfo );
		auto attributeDescriptions = getAttributeDescriptions24 ();

		pipe = device->createGraphPipeline ( getBindingDescription24 (), attributeDescriptions, device->descriptorSetLayout, "shaders/total_v.spv", "shaders/total_f.spv" );


		vertex = device->createDataBuffer ( (void*) info.vertices.data (), sizeof ( info.vertices[0] ) * info.vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
		indices = device->createDataBuffer ( (void*) info.indices.data (), sizeof ( info.indices[0] ) * info.indices.size (), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
		indicesSizes = info.indices.size ();
		//device->createDataBuffer ( (void*) vertices.data (), sizeof ( vertices[0] ) * vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
		this->ubo = ubo;
		this->ubo2 = ubo2;
		this->light = lightUBO;
		this->me = meUBO;

		pipeline = pipe.pipeline;
		pipelineLayout = pipe.pipelineLayout;

	};

	void VulkanEntity::render ( UniformBufferObject ubo ) {

		auto currentFrame = device->currentFrame;


		Frame frame = device->frames[currentFrame];
		auto commandBuffer = frame.commandBuffers;
		MeUBO me = {};
		me.color = glm::vec3 ( 0.99f, 0.026f, 0.011f );
		me.color2 = glm::vec3 ( 0.5f, 0.0f, 1.0f );
		me.intensity = 2.1f - 0.11f;

		
		LightUBO l = { };

		l.position = glm::vec3 ( 0.6f, 0.05f, 0.1f );
		l.color = glm::vec3 ( 0.6f, 0.3f, 0.0f );
		l.intensity = 0.5f;


		UniformBufferObject ubo2 {};


		glm::mat4 translation = glm::translate ( glm::mat4 ( 1.0f ), l.position );
		//glm::mat4 rotation = glm::rotate ( glm::mat4 ( 1.0f ), time * 0.1f * glm::radians ( 90.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );

		glm::mat4 rotationMat = glm::rotate ( glm::mat4 ( 1.0f ), l.color.z, glm::vec3 ( 0.0f, 0.0f, 1.0f ) ) *
			glm::rotate ( glm::mat4 ( 1.0f ), l.color.y, glm::vec3 ( 0.0f, 1.0f, 0.0f ) ) *
			glm::rotate ( glm::mat4 ( 1.0f ), l.color.x, glm::vec3 ( 1.0f, 0.0f, 0.0f ) );
		ubo2.model = translation * rotationMat;


		//ubo.model = /* rotation * */  translation;


		// Posición de la cámara arriba en el eje Z
		glm::vec3 cameraPos = glm::vec3 ( 0.0f, 0.0f, -5.0f );

		//glm::vec3 cameraPos = glm::vec3 ( 0.0f, 0.0f, 5.0f );

		// Punto al que está mirando la cámara (el origen en este caso)
		glm::vec3 target = glm::vec3 ( 0.0f, 0.0f, 0.0f );
		// Dirección "up" (hacia el eje Y)
		//glm::vec3 up = camera.up; // glm::vec3 ( 0.0f, 1.0f, 0.0f );
		glm::vec3 up = glm::vec3 ( 0.0f, -25.0f, 0.0f );

		ubo2.view = glm::lookAt ( cameraPos, target, up );

		//ubo.view = glm::lookAt ( glm::vec3 ( 2.0f, 2.0f, 2.0f ), glm::vec3 ( 0.0f, 0.0f, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
		ubo2.proj = glm::perspective ( glm::radians ( 45.0f ), 1300 / (float) 600, 0.1f, 100.0f );


		memcpy ( this->light[currentFrame].buffersMapped, &l, sizeof ( l ) );
		memcpy ( this->me[currentFrame].buffersMapped, &me, sizeof ( me ) );
		memcpy ( this->ubo[currentFrame].buffersMapped, &ubo, sizeof ( ubo ) );
		//memcpy ( entity->ubo2[currentFrame].buffersMapped, &ubo2, sizeof ( ubo2 ) );


		VkBuffer vertexBuffers[] = { vertex.buffer };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline );
		vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );
		//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindIndexBuffer ( commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT32 );

		vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr );
		vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(indicesSizes), 1, 0, 0, 0 );

		//entity->render ( ubo );

	}
}


