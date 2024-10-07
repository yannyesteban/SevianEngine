#pragma once

#include "System.h"
#include "Scene.h"
#include "Entity.h"
#include "Models.h"
//#include "VulkanProperty.h"
//#include "VulkanDevice.h"
#include "Component.h"
#include <RendererFactory.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace SEVIAN {
	class LightSystem : public System
	{
	private:

	public:
		LightSystem ();
		void init ( std::vector<std::shared_ptr<Entity>>& entities ) override;
		void update ( std::vector<std::shared_ptr<Entity>>& entities ) override;
	};

}