#include "LightEntity.h"


namespace SEVIAN {
    std::shared_ptr<Entity> createLight ( Scene& scene, LightComponent light ) {
		auto entity = scene.createEntity ();
		
		entity->addComponent<LightComponent> ( light );
		return entity;
    }

}
