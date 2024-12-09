#pragma once
#include <vector>
#include <memory>
#include "Entity.h"

namespace SEVIAN {


	class System
	{
	public:
		virtual ~System () = default;
		virtual void update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) = 0;
		virtual void init ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) = 0;
	};

}