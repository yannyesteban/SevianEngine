#include "CameraEntity.h"
namespace SEVIAN {
    std::shared_ptr<Entity> createCamera ( Scene& scene, CameraComponent camera ) {
		auto entity = scene.createEntity ();
		// entity->addComponent<MaterialComponent> ( material );

		//CameraComponent camera { };
		camera.position = glm::vec3 ( 0.0f, 0.0f, 5.0f );
		camera.front = glm::vec3 ( 0.0f, 0.0f, -5.0f );
		camera.up = glm::vec3 ( 0.0f, 1.0f, 0.0f );

		entity->addComponent<CameraComponent> ( camera );
		return entity;
    }


	std::shared_ptr<Entity> createCamera2D ( Scene& scene, CameraComponent info ) {

		Camera2DComponent camera { };

		auto entity = scene.createEntity ();

		camera.position = info.position;
		camera.front = info.front;
		camera.up = info.up;

		camera.width = info.width;
		camera.height = info.height;

		auto deltaX = static_cast<float>(camera.width) / 2;
		auto deltaY = static_cast<float>(camera.height) / 2;
		
		camera.view = glm::lookAt ( 
			camera.position,  // Cámara desde (0, 0, 2)
			camera.front,  // Mira al origen
			camera.up ); // "Arriba" en la dirección Y
		camera.proj = glm::ortho ( -deltaX, deltaX, -deltaY, deltaY, -1.0f, 1.0f );
		//camera.proj = glm::ortho ( -deltaX, deltaX, -deltaY, deltaY );
		camera.proj[1][1] *= -1;  // Solo se necesita si estás usando Vulkan o OpenGL con un espacio de recorte diferente
		camera.view = glm::translate ( glm::mat4 ( 1.0f ), glm::vec3 ( -glm::vec2 ( 0.0f, 0.0f ), 0.0f ) );



		camera.proj = glm::ortho ( 0.0f, deltaX * 2.0f, -deltaY, deltaY, -1.0f, 1.0f );
		camera.proj = glm::ortho ( 0.0f, deltaX * 2.0f, -deltaY, deltaY, -1.0f, 1.0f );
		camera.proj = glm::ortho ( 0.0f, deltaX * 2.0f, 0.0f, deltaY * 2.0f, -1.0f, 1.0f );
		camera.proj = glm::ortho ( 0.0f, deltaX * 2.0f, deltaY * 2, 0.0f, -1.0f, 1.0f );

		if (1 == 2) {
			camera.proj = glm::ortho ( -deltaX, deltaX, 0.0f, deltaY * 2.0f );
			//camera.proj[1][1] *= -1;  // Solo se necesita si estás usando Vulkan o Open
		}
		else {
			camera.proj = glm::ortho ( 0.0f, deltaX * 2.0f, 0.0f, deltaY * 2.0f );
			//camera.proj[1][1] *= -1;  // Solo se necesita si estás usando Vulkan o Open
		}
		

		entity->addComponent<Camera2DComponent> ( camera );

		return entity;
	}
}