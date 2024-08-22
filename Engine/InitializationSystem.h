#pragma once
#include "System.h"
#include "Scene.h"
#include "Entity.h"
//#include "VulkanDevice.h"
#include "Component.h"
#include "RendererFactory.h"

namespace SEVIAN {

    class InitializationSystem : public System
    {
        std::shared_ptr<RenderInterface> renderer;

    public:
        InitializationSystem ( std::shared_ptr<RenderInterface>  r ) : renderer ( r ) { }
       
        void init ( std::vector<std::shared_ptr<Entity>>& entities ) override;
        void update ( std::vector<std::shared_ptr<Entity>>& entities ) override;

    };
}