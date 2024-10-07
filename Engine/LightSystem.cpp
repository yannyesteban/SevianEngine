#include "LightSystem.h"



namespace SEVIAN {

	LightSystem::LightSystem () {
	}

	void SEVIAN::LightSystem::init ( std::vector<std::shared_ptr<Entity>>& entities ) {
	}

	void SEVIAN::LightSystem::update ( std::vector<std::shared_ptr<Entity>>& entities ) {

		for (auto& entity : entities) {

			auto light = entity->getComponent<LightComponent> ();
			bool active = false;

			if (light) {

				if (Tools::isKeyPressed ( Key::Z )) {

					light->position.z = light->position.z + 0.002f;
				}
				if (Tools::isKeyPressed ( Key::X )) {
					light->position.z = light->position.z - 0.002f;
				}


				if (Tools::isKeyPressed ( Key::A )) {

					light->position.x = light->position.x + 0.002f;
				}
				if (Tools::isKeyPressed ( Key::S )) {
					light->position.x = light->position.x - 0.002f;
				}

				
			}
		}
	}

}
