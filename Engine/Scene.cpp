#include "Scene.h"

namespace SEVIAN {

	std::shared_ptr<Entity> Scene::createEntity () {
		static EntityID nextID = 0;
		auto entity = std::make_shared<Entity> ( nextID++ );
		entities.push_back ( entity );
		return entity;
	}

	const std::vector<std::shared_ptr<Entity>>& Scene::getEntities () const {

		return entities;

	}

	void Scene::addSystem ( std::shared_ptr<System> system ) {
		systems.push_back ( std::move ( system ) );
	}

	void Scene::addStartSystem ( std::shared_ptr<System> system ) {
		startSystems.push_back ( std::move ( system ) );
	}
	void Scene::runSystems () {
		for (auto& system : systems) {
			system->update ( entities, 0 );
		}
	}
	void Scene::runStartSystems () {
		for (auto& system : startSystems) {
			system->update ( entities, 0 );
		}
	}
	void Scene::init ( float deltaTime ) {
		for (auto& system : systems) {
			system->init ( entities, deltaTime );
		}
	}
	void Scene::update ( float deltaTime ) {
		for (auto& system : systems) {
			system->update ( entities, deltaTime );
		}
	}
}
