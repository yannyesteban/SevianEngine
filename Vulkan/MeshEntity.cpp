#include "MeshEntity.h"

VkVertexInputBindingDescription getBindingDescriptionGeneric () {
	VkVertexInputBindingDescription bindingDescription {};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof ( SEVIAN::Vertex );
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}
std::vector<VkVertexInputAttributeDescription> getAttributeDescriptionsGeneric () {
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

namespace VULKAN {


	MeshEntity::MeshEntity ( Device* device, TextureManager* textureManager, Info3D info, VkPipelineLayout pipelineLayout, VkPipeline pipeline, VkDescriptorSetLayout bufDescriptorSetLayout, VkDescriptorSetLayout texDescriptorSetLayout )
			:device(device), textureManager ( textureManager ), info ( info ), pipelineLayout( pipelineLayout ), pipeline( pipeline ), bufDescriptorSetLayout( bufDescriptorSetLayout ), texDescriptorSetLayout ( texDescriptorSetLayout ) {
		
		textureManager->add ( info.texture, info.path );
		auto texture = textureManager->get ( info.texture );
		
		this->ubo = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );

		std::vector<DSInfo> bufDSInfo;
		bufDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ubo, sizeof ( UniformBufferObject ), VK_NULL_HANDLE, VK_NULL_HANDLE, 0 } );

		std::vector<DSInfo> texDSInfo;
		texDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ubo, sizeof ( UniformBufferObject ), texture->imageView, texture->sampler, 0 } );

		this->bufDescriptorSets = device->createDescriptorSets ( bufDescriptorSetLayout, bufDSInfo );
		this->texDescriptorSets = device->createDescriptorSets ( texDescriptorSetLayout, texDSInfo );

		vertex = device->createDataBuffer ( (void*) info.vertices.data (), sizeof ( info.vertices[0] ) * info.vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
		indices = device->createDataBuffer ( (void*) info.indices.data (), sizeof ( info.indices[0] ) * info.indices.size (), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
		
		indicesSizes = info.indices.size ();

	}

	MeshEntity::MeshEntity ( Device* device, TextureManager* textureManager, Info3D info, Propertys propertys )
		:device ( device ), textureManager ( textureManager ), info ( info ), propertys( propertys ){

		textureManager->add ( info.texture, info.path );
		auto texture = textureManager->get ( info.texture );
		


		ubo = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );
		ubo1 = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );

		std::vector<DSInfo> bufDSInfo;
		bufDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ubo, sizeof ( UniformBufferObject ), VK_NULL_HANDLE, VK_NULL_HANDLE, 0 } );

		std::vector<DSInfo> texDSInfo;
		texDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ubo, sizeof ( UniformBufferObject ), texture->imageView, texture->sampler, 0 } );
		texDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ubo, sizeof ( UniformBufferObject ), device->depthResource.imageView, device->shadowMapSampler, 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL } );


		
		std::vector<DSInfo> shadowDSInfo;
		shadowDSInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ubo1, sizeof ( UniformBufferObject ), VK_NULL_HANDLE, VK_NULL_HANDLE, 0 } );

		this->bufDescriptorSets = device->createDescriptorSets ( propertys.bufDescriptorSetLayout, bufDSInfo );
		this->texDescriptorSets = device->createDescriptorSets ( propertys.texDescriptorSetLayout, texDSInfo );
		this->shadowDescriptorSets = device->createDescriptorSets ( propertys.shadowDescriptorSetLayout, shadowDSInfo );

		vertex = device->createDataBuffer ( (void*) info.vertices.data (), sizeof ( info.vertices[0] ) * info.vertices.size (), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
		indices = device->createDataBuffer ( (void*) info.indices.data (), sizeof ( info.indices[0] ) * info.indices.size (), VK_BUFFER_USAGE_INDEX_BUFFER_BIT );

		indicesSizes = info.indices.size ();

		pipelineLayout = propertys.pipelineLayout;
		pipeline = propertys.pipeline;

		shadowPipelineLayout = propertys.shadowPipelineLayout;
		shadowPipeline = propertys.shadowPipeline;
	}
	
	void MeshEntity::render ( UniformBufferObject ubo ) {
		auto currentFrame = device->currentFrame;

		Frame frame = device->frames[currentFrame];
		auto commandBuffer = frame.commandBuffers;
		
		memcpy ( this->ubo[currentFrame].buffersMapped, &ubo, sizeof ( ubo ) );
		
		VkBuffer vertexBuffers[] = { vertex.buffer };
		VkDeviceSize offsets[] = { 0 };
		
		vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline );
		vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );
		//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindIndexBuffer ( commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT32 );

		vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &bufDescriptorSets[currentFrame], 0, nullptr );
		vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 1, 1, &texDescriptorSets[currentFrame], 0, nullptr );
		vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(indicesSizes), 1, 0, 0, 0 );
	}
	void MeshEntity::ShadowRender ( UniformBufferObject ubo ) {
		auto currentFrame = device->currentFrame;

		Frame frame = device->frames[currentFrame];
		auto commandBuffer = frame.commandBuffers;

		memcpy ( this->ubo1[currentFrame].buffersMapped, &ubo, sizeof ( ubo ) );

		VkBuffer vertexBuffers[] = { vertex.buffer };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shadowPipeline );
		vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );
		vkCmdBindIndexBuffer ( commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT32 );
		vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shadowPipelineLayout, 0, 1, &shadowDescriptorSets[currentFrame], 0, nullptr );
		
		vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(indicesSizes), 1, 0, 0, 0 );
	}
}