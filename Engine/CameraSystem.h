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
	class CameraSystem : public System
	{
	private:

	public:
		CameraSystem ( );
		void init ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;
		void update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;
	};

}

namespace SEVIAN {
	class Camera2DSystem : public System
	{
	private:

	public:
		Camera2DSystem () { };
		void init ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;
		void update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;
	};

}