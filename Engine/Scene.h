#pragma once
#include "Entity.h"
#include "System.h"

namespace SEVIAN {
	class Scene
	{
	private:
		std::vector<std::shared_ptr<Entity>> entities;
		std::vector<std::shared_ptr<System>> systems;
		std::vector<std::shared_ptr<System>> startSystems;
	public:
		std::shared_ptr<Entity> createEntity ();
		const std::vector<std::shared_ptr<Entity>>& getEntities () const;
		void addSystem ( std::shared_ptr<System> system );
		void addStartSystem ( std::shared_ptr<System> system );
		void runSystems ();
		void runStartSystems ();
		void init ();
		void update ();
	};

}

