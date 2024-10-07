#include "CameraEntity.h"
namespace SEVIAN {
    std::shared_ptr<Entity> createCamera ( Scene& scene, CameraComponent camera ) {
		auto entity = scene.createEntity ();
		// entity->addComponent<MaterialComponent> ( material );

		//CameraComponent camera { };
		camera.position = glm::vec3 ( 0.1f, 0.0f, 5.0f );
		camera.front = glm::vec3 ( 0.0f, 0.0f, 0.0f );
		camera.up = glm::vec3 ( 0.0f, 1.0f, 0.0f );

		entity->addComponent<CameraComponent> ( camera );
		return entity;
    }
}