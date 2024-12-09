#include "LightSystem.h"



namespace SEVIAN {

	LightSystem::LightSystem () {
	}

	void SEVIAN::LightSystem::init ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) {
	}

	void SEVIAN::LightSystem::update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) {

		for (auto& entity : entities) {

			auto light = entity->getComponent<LightComponent> ();
			bool active = false;

			if (light) {
				bool keyPressed = false;

				if (Tools::isKeyPressed ( Key::Z )) {
					light->position.z = light->position.z - 0.002f;
					keyPressed = true;
				}
				if (Tools::isKeyPressed ( Key::X )) {
					light->position.z = light->position.z + 0.002f;
					keyPressed = true;
				}
				
				if (Tools::isKeyPressed ( Key::Q )) {
					light->position.x = light->position.x - 0.002f;
					keyPressed = true;
				}
				if (Tools::isKeyPressed ( Key::W )) {
					light->position.x = light->position.x + 0.002f;
					keyPressed = true;
				}
				
				if (Tools::isKeyPressed ( Key::A )) {
					light->position.y = light->position.y - 0.002f;
					keyPressed = true;
				}
				if (Tools::isKeyPressed ( Key::S )) {
					light->position.y = light->position.y + 0.002f;
					keyPressed = true;
				}



				if (Tools::isKeyPressed ( Key::K1 )) {
					light->near_plane = light->near_plane - 0.01f;
					keyPressed = true;
				}
				if (Tools::isKeyPressed ( Key::K2 )) {
					light->near_plane = light->near_plane + 0.01f;
					keyPressed = true;
				}

				if (Tools::isKeyPressed ( Key::K3 )) {
					light->far_plane = light->far_plane - 0.01f;
					keyPressed = true;
				}
				if (Tools::isKeyPressed ( Key::K4 )) {
					light->far_plane = light->far_plane + 0.01f;
					keyPressed = true;
				}
				if (Tools::isKeyPressed ( Key::K5 )) {
					light->shadowMapSize = light->shadowMapSize - 0.01f;
					keyPressed = true;
				}
				if (Tools::isKeyPressed ( Key::K6 )) {
					light->shadowMapSize = light->shadowMapSize + 0.01f;
					keyPressed = true;
				}

				if (Tools::isKeyPressed ( Key::E )) {
					light->intensity = light->intensity - 0.01f;
					keyPressed = true;
				}
				if (Tools::isKeyPressed ( Key::R )) {
					light->intensity = light->intensity + 0.01f;
					keyPressed = true;
				}


			
			

				light->view = glm::lookAt ( light->position, light->direction, light->up );

				// Proyección ortográfica para simular las sombras de la luz (ajusta los parámetros según tu escena)
				//float near_plane = 0.1f, far_plane = 50.0f;
				//float shadowMapSize = 5.0f; // Ajusta según la distancia que quieras cubrir
			

				light->proj = glm::ortho ( -light->shadowMapSize, light->shadowMapSize, -light->shadowMapSize, light->shadowMapSize, light->near_plane, light->far_plane );

				//light->proj[0][0] *= -1;


				//light->proj = glm::perspective ( glm::radians ( 45.0f ), 1300 / (float) 600, 0.1f, 100.0f );
				//light->proj[1][1] *= -1;

				if (keyPressed) {
					std::cout << "\n\n=========================\nLight position: (" << light->position.x << ", " << light->position.y << ", " << light->position.z << ")" << std::endl;
					std::cout << "Near plane: " << light->near_plane << std::endl;
					std::cout << "Far plane: " << light->far_plane << std::endl;
					std::cout << "Shadow Map Size: " << light->shadowMapSize << std::endl;
				}
			}
		}
	}

}
