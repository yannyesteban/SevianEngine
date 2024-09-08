#include "RaceSystem.h"

void SEVIAN::RaceSystem::init ( std::vector<std::shared_ptr<Entity>>& entities ) {
}

void SEVIAN::RaceSystem::update ( std::vector<std::shared_ptr<Entity>>& entities ) {



	for (auto& entity : entities) {



		auto name = entity->getComponent<NameComponent> ();

		auto input = entity->getComponent<InputComponent> ();
		auto position = entity->getComponent<PositionComponent> ();
		auto rotation = entity->getComponent<RotationComponent> ();

		auto camera = entity->getComponent<CameraComponent> ();
		bool active = false;


		

		if (camera) {
			

			if (Tools::isKeyPressed ( Key::H )) {
				std::cout << Tools::isKeyPressed ( Key::N1 ) << " ... ... ... .. ..\n";
				//auto camera = Tools::getCamera ();

				auto camera = Tools::getCamera ();
				camera->position.z = camera->position.z + 0.1f;
			}
			if (Tools::isKeyPressed ( Key::B )) {
				std::cout << Tools::isKeyPressed ( Key::N1 ) << " ... ... ... .. ..\n";
				//auto camera = Tools::getCamera ();

				auto camera = Tools::getCamera ();
				camera->position.z = camera->position.z - 0.1f;
			}

			//camera->position =  glm::vec3 ( 0.0f, 0.0f, -8.0f );
		}

		if (name) {

			if (Tools::isKeyPressed ( Key::N0 ) || Tools::isKeyPressed ( Key::N1 ) || Tools::isKeyPressed ( Key::N2 )
				|| Tools::isKeyPressed ( Key::N3 ) || Tools::isKeyPressed ( Key::N4 ) || Tools::isKeyPressed ( Key::N5 )
				|| Tools::isKeyPressed ( Key::N6 ) || Tools::isKeyPressed ( Key::N7 ) || Tools::isKeyPressed ( Key::N8 ) || Tools::isKeyPressed ( Key::N9 )
				) {



				if (Tools::isKeyPressed ( name->name )) {
					name->active = true;

				}
				else {
					name->active = false;
				}


			}



			active = name->active;
			//std::cout << "MyName is " << name << "\n";
		}

		if (!active) {
			//std::cout << Tools::isKeyPressed ( Key::N1 ) << "\n";
			continue;// return;
		}

		if (rotation) {
			if (Tools::isKeyPressed ( Key::Y )) {
				
				rotation->rotation.y += 0.001;
			}
			if (Tools::isKeyPressed ( Key::U )) {
				
				rotation->rotation.z += 0.001;
			}
			if (Tools::isKeyPressed ( Key::T )) {
				

				rotation->rotation.x += 0.001;
			}
		}

		if (/*input && */  position) {

			if (Tools::isKeyPressed ( Key::A ) || Tools::isKeyPressed ( Key::LEFT )) {
				position->position.x -= 0.001;
			}

			if (Tools::isKeyPressed ( Key::S ) || Tools::isKeyPressed ( Key::RIGHT )) {
				position->position.x += 0.001;
			}

			if (Tools::isKeyPressed ( Key::W ) || Tools::isKeyPressed ( Key::UP )) {
				position->position.y += 0.001;
			}

			if (Tools::isKeyPressed ( Key::Z ) || Tools::isKeyPressed ( Key::DOWN )) {
				position->position.y -= 0.001;
			}


			/*
			if (input->keyStates[static_cast<size_t>(Key::A)]) {
				position->position.x -= 0.001;
			}

			if (input->keyStates[static_cast<size_t>(Key::RIGHT)]) {

				auto camera = Tools::getCamera ();
				camera->position = glm::vec3 ( 0.0f, 0.0f, camera->position.z + 0.01 );


			}

			if (input->keyStates[static_cast<size_t>(Key::LEFT)]) {
				auto camera = Tools::getCamera ();
				camera->position = glm::vec3 ( 0.0f, 0.0f, camera->position.z - 0.01 );
			}


			if (input->keyStates[static_cast<size_t>(Key::UP)]) {
				auto camera = Tools::getCamera ();
				camera->up = glm::vec3 ( 0.0f, camera->up.y - 0.01, 0.0f );
			}


			if (input->keyStates[static_cast<size_t>(Key::DOWN)]) {
				auto camera = Tools::getCamera ();
				camera->up = glm::vec3 ( 0.0f, camera->up.y - 0.01, 0.0f );
			}

			*/

		}
	}
}
