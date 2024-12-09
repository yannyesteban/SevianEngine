#pragma once

#include <memory>
#include "models.h" 
#include "Scene.h"

namespace SEVIAN { 

	std::shared_ptr<Entity> createActor ( Scene& scene );

}


