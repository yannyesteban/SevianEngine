#pragma once
//#include "Tools.h"
#include "System.h"
#include "Scene.h"
#include "Entity.h"
#include "Models.h"
#include "Component.h"
#include <RendererFactory.h>
#include "IInputManager.h"

namespace SEVIAN {

    class InputSystem : public System
    {
        //int device;
        std::shared_ptr<RENDERER::RenderInterface> renderer;
        std::shared_ptr<INPUT::IInputManager> manager;

    public:
        InputSystem ( std::shared_ptr<RENDERER::RenderInterface>  r ) : renderer ( r ) { }
        InputSystem ( std::shared_ptr<INPUT::IInputManager> manager ) : manager ( manager ) { }
        void init ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;
        void update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) override;

    };
}

