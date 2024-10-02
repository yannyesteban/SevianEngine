#include "MeshManager.h"

namespace VULKAN {
	MeshManager::MeshManager ( Device* device, TextureManager* textureManager ) : device ( device ), textureManager ( textureManager ) {
		

		ubo = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );
		//ubo2 = device->createUniformBuffer ( device->frames, sizeof ( UniformBufferObject ) );
		light = device->createUniformBuffer ( device->frames, sizeof ( LightUBO ) );
		me = device->createUniformBuffer ( device->frames, sizeof ( MeUBO ) );

		buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ubo, sizeof ( UniformBufferObject ), VK_NULL_HANDLE, VK_NULL_HANDLE, 0 } );
		buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, light, sizeof ( LightUBO ), VK_NULL_HANDLE, VK_NULL_HANDLE, 1 } );
		buffersInfo.push_back ( { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, me, sizeof ( MeUBO ), VK_NULL_HANDLE, VK_NULL_HANDLE, 2 } );
		
		descriptorSetLayout = device->createDescriptorSetLayout ( buffersInfo );
		descriptorSets = device->createDescriptorSets ( descriptorSetLayout, buffersInfo );
		
	}
	std::unique_ptr<SEVIAN::Entity3D> MeshManager::create ( SEVIAN::Info3D info ) {
		
		auto entity = std::make_unique<MeshEntity> ( device, textureManager, descriptorSetLayout, descriptorSets, pipeline, info );
		
		entity->ubo = ubo;
		entity->light = light;
		entity->me = me;
		
		return std::unique_ptr<Entity3D> ( std::move ( entity ) );
		
	}
}

