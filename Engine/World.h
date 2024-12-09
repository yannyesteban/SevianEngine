#pragma once
#include "Scene.h"

namespace SEVIAN {
    class World
    {
        std::vector<std::unique_ptr<Scene>> scenes1;
        std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
        std::vector<std::unique_ptr<System>> systems;

        std::shared_ptr<Scene> scene;

    public:
        Scene* createScene ( const std::string& name) {
            scenes[name] = std::make_unique<Scene> ();
            //scenes.push_back ( std::make_unique<Scene> () );
            scene = scenes[name];
            return scenes[name].get();
        }
        void addSystem ( std::unique_ptr<System> system ) {
            systems.push_back ( std::move ( system ) );
        }

        void update ( float deltaTime ) {
             scene->update ( deltaTime );
            
        }
    };
}


