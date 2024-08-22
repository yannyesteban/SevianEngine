#pragma once
//#include "Tools.h"
#include "System.h"
#include "Scene.h"
#include "Entity.h"
#include "Models.h"
#include "Component.h"
#include <RendererFactory.h>

namespace SEVIAN {

    class InputSystem : public System
    {
        //int device;
        std::shared_ptr<RenderInterface> renderer;

    public:
        InputSystem ( std::shared_ptr<RenderInterface>  r ) : renderer ( r ) { }
        void init ( std::vector<std::shared_ptr<Entity>>& entities ) override;
        void update ( std::vector<std::shared_ptr<Entity>>& entities ) override;

    };
}

