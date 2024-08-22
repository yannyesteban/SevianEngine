#pragma once


#include "System.h"
#include "Scene.h"
#include "Entity.h"
#include "Models.h"
//#include "VulkanDevice.h"
#include "Component.h"
#include <RendererFactory.h>

namespace SEVIAN {

    class RaceSystem : public System
    {
        

    public:
        RaceSystem (  )  { }
        void init ( std::vector<std::shared_ptr<Entity>>& entities ) override;
        void update ( std::vector<std::shared_ptr<Entity>>& entities ) override;

    };
}

