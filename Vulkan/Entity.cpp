#include "Entity.h"



namespace VULKAN { 

	void Entity::render ( UniformBufferObject ubo ) {
		std::cout << "HOLA MUNDO RENDER()\n";
	}
	void Entity::ShadowRender ( ::SEVIAN::UniformBufferObject ubo ) {
	}
	void Entity::ShadowRender ( ::SEVIAN::UniformDataDept ubo ) {
	}
}