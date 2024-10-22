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
			bool keyPressed = false;

			// Rotar alrededor del origen
			if (Tools::isKeyPressed ( Key::O )) {
				camera->angleY -= 0.001f; // Rotar a la izquierda
				keyPressed = true;
			}
			if (Tools::isKeyPressed ( Key::P )) {
				camera->angleY += 0.001f; // Rotar a la derecha
				keyPressed = true;
			}
			if (Tools::isKeyPressed ( Key::K )) {
				camera->angleX += 0.001f; // Rotar hacia arriba
				keyPressed = true;
			}
			if (Tools::isKeyPressed ( Key::L )) {
				camera->angleX -= 0.001f; // Rotar hacia abajo
				keyPressed = true;
			}

			// Zoom
			if (Tools::isKeyPressed ( Key::PLUS )) {
				camera->distance -= 0.01f; // Aumentar el zoom (acercarse)
				keyPressed = true;
			}
			if (Tools::isKeyPressed ( Key::MINUS )) {
				camera->distance += 0.01f; // Disminuir el zoom (alejarse)
				keyPressed = true;
			}

			// Calcular la posición de la cámara
			camera->position.x = camera->distance * sin ( camera->angleY ) * cos ( camera->angleX );
			camera->position.y = camera->distance * sin ( camera->angleX );
			camera->position.z = camera->distance * cos ( camera->angleY ) * cos ( camera->angleX );

			// Configurar el objetivo de la cámara (mirando hacia el origen)
			camera->target = glm::vec3 ( 0.0f, 0.0f, 0.0f );

			// Actualizar matrices de vista y proyección
			camera->view = glm::lookAt ( camera->position, camera->target, camera->up );
			camera->proj = glm::perspective ( glm::radians ( camera->fov ), camera->width / camera->height, 0.1f, 100.0f );
			camera->proj[1][1] *= -1; // Invertir el eje Y en proyección si es necesario
		}

		if (camera && 7 == 4) {
			bool keyPressed = false;

			// Control de rotación
			if (Tools::isKeyPressed ( Key::K )) {
				camera->yaw -= 0.1f; // Rotación hacia la izquierda
				keyPressed = true;
			}
			if (Tools::isKeyPressed ( Key::L )) {
				camera->yaw += 0.1f; // Rotación hacia la derecha
				keyPressed = true;
			}
			if (Tools::isKeyPressed ( Key::O )) {
				camera->pitch += 0.1f; // Rotación hacia arriba
				keyPressed = true;
			}
			if (Tools::isKeyPressed ( Key::P )) {
				camera->pitch -= 0.1f; // Rotación hacia abajo
				keyPressed = true;
			}

			if (Tools::isKeyPressed ( Key::K )) {
				camera->angle -= 0.01f; // Rotar a la izquierda
			}
			if (Tools::isKeyPressed ( Key::L )) {
				camera->angle += 0.01f; // Rotar a la derecha
			}



			// Control de zoom

			if (Tools::isKeyPressed ( Key::PLUS )) {
				camera->position.z = camera->position.z - 0.003f;
				bool keyPressed = true;
			}
			if (Tools::isKeyPressed ( Key::MINUS )) {
				camera->position.z = camera->position.z + 0.003f;
				bool keyPressed = true;
			}
			/*if (Tools::isKeyPressed (Key::O)) {
				camera->position.x = camera->position.x - 0.003f;
				bool keyPressed = true;
			}
			if (Tools::isKeyPressed ( Key::P )) {
				camera->position.x = camera->position.x + 0.003f;
				bool keyPressed = true;
			}

			if (Tools::isKeyPressed ( Key::K )) {

				camera->position.y = camera->position.y - 0.003f;
				bool keyPressed = true;
			}
			if (Tools::isKeyPressed ( Key::L )) {
				camera->position.y = camera->position.y + 0.003f;
				bool keyPressed = true;
			}*/
			// Calcular la nueva dirección de la cámara usando yaw y pitch
			glm::vec3 front;
			front.x = cos ( glm::radians ( camera->yaw ) ) * cos ( glm::radians ( camera->pitch ) );
			front.y = sin ( glm::radians ( camera->pitch ) );
			front.z = sin ( glm::radians ( camera->yaw ) ) * cos ( glm::radians ( camera->pitch ) );
			camera->target = glm::normalize ( front ); // Actualizar el target de la cámara


			// Calcular la nueva posición de la cámara
			//camera->position.x = camera->distanceFromOrigin * cos ( camera->angle );
			//camera->position.z = camera->distanceFromOrigin * sin ( camera->angle );
			//camera->position.y = camera->position.y; // Puedes ajustar la altura aquí

			//camera->target = glm::vec3 ( 0.0f, 0.0f, 0.0f ); // Siempre mirando hacia el origen

			// Actualizar la matriz de vista

			camera->view = glm::lookAt ( camera->position, camera->position +  camera->target, camera->up );
			camera->proj = glm::perspective ( glm::radians ( camera->fov ), camera->width / camera->height, 0.1f, 100.0f );
			camera->proj[1][1] *= -1;
		}
	}
}
