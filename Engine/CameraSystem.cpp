#include "CameraSystem.h"

SEVIAN::CameraSystem::CameraSystem () {
}

void SEVIAN::CameraSystem::init ( std::vector<std::shared_ptr<Entity>>& entities ) {
}

void SEVIAN::CameraSystem::update ( std::vector<std::shared_ptr<Entity>>& entities ) {

	for (auto& entity : entities) {

		auto camera = entity->getComponent<CameraComponent> ();
		bool active = false;

		if (camera) {

			if (Tools::isKeyPressed ( Key::Q )) {
	
				camera->position.z = camera->position.z + 0.1f;
			}
			if (Tools::isKeyPressed ( Key::W )) {
				camera->position.z = camera->position.z - 0.1f;
			}

			camera->view = glm::lookAt ( camera->position, camera->target, camera->up );
			camera->proj = glm::perspective ( glm::radians ( 45.0f ), 1300 / (float) 600, 0.1f, 100.0f );
			camera->proj[1][1] *= -1;
		}
	}
}
