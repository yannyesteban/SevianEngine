#pragma once
#include "System.h"
#include "Scene.h"
#include "Entity.h"
#include "Models.h"

#include "Component.h"
//#include "TransformSystem.h"
//#include <RendererFactory.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TransformSystem.h"
namespace SEVIAN { 


	


	class AnimationSystem : public System 
		
	{
	public:
		//AnimationSystem (  ) { }
		void init ( std::vector<std::shared_ptr<Entity>>& entitie, float deltaTimes ) override;
		void update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;
	};

}


