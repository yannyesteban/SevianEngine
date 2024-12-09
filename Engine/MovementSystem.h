#pragma once
#include "System.h"
#include "Scene.h"
#include "Entity.h"
#include "Models.h"
//#include "VulkanDevice.h"
#include "Component.h"
#include <RendererFactory.h>

namespace SEVIAN {

    class MovementSystem : public System
    {
        //int device;
        std::shared_ptr<RenderInterface> renderer;

    public:
        MovementSystem ( std::shared_ptr<RenderInterface>  r ) : renderer ( r ) { }
        void init ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;
        void update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;

    };
}


