#include "RaceSystem.h"

void SEVIAN::RaceSystem::init ( std::vector<std::shared_ptr<Entity>>& entities ) {
}

void SEVIAN::RaceSystem::update ( std::vector<std::shared_ptr<Entity>>& entities ) {

   
    
    for (auto& entity : entities) {
       

       
        auto name = entity->getComponent<NameComponent> ();
        
        auto input = entity->getComponent<InputComponent> ();
        auto position = entity->getComponent<PositionComponent> ();

        auto camera = entity->getComponent<CameraComponent> ();
        bool active = false;
        if (name) {
           
            if (Tools::isKeyPressed ( Key::N1 ) || Tools::isKeyPressed ( Key::N2 ) 
                || Tools::isKeyPressed ( Key::N3 ) || Tools::isKeyPressed ( Key::N4 ) || Tools::isKeyPressed ( Key::N5 )) {

               

                if (Tools::isKeyPressed ( name->name )) {
                    name->active = true;

                } else {
                    name->active = false;
                }


            }

           

            active = name->active;
            //std::cout << "MyName is " << name << "\n";
        }

        if (!active) {
            //std::cout << Tools::isKeyPressed ( Key::N1 ) << "\n";
            continue;// return;
        }

        if (camera) {
            //camera->position =  glm::vec3 ( 0.0f, 0.0f, -8.0f );
        }

        if (/*input && */  position) {
           
            if (Tools::isKeyPressed ( Key::A ) || Tools::isKeyPressed ( Key::LEFT )) {
                position->position.x -= 0.001;
            }

            if (Tools::isKeyPressed ( Key::S ) || Tools::isKeyPressed ( Key::RIGHT )) {
                position->position.x += 0.001;
            }

            if (Tools::isKeyPressed ( Key::W ) || Tools::isKeyPressed ( Key::UP )) {
                position->position.y += 0.001;
            }

            if (Tools::isKeyPressed ( Key::Z ) || Tools::isKeyPressed ( Key::DOWN )) {
                position->position.y -= 0.001;
            }


            /*
            if (input->keyStates[static_cast<size_t>(Key::A)]) {
                position->position.x -= 0.001;
            }

            if (input->keyStates[static_cast<size_t>(Key::RIGHT)]) {

                auto camera = Tools::getCamera ();
                camera->position = glm::vec3 ( 0.0f, 0.0f, camera->position.z + 0.01 );


            }

            if (input->keyStates[static_cast<size_t>(Key::LEFT)]) {
                auto camera = Tools::getCamera ();
                camera->position = glm::vec3 ( 0.0f, 0.0f, camera->position.z - 0.01 );
            }


            if (input->keyStates[static_cast<size_t>(Key::UP)]) {
                auto camera = Tools::getCamera ();
                camera->up = glm::vec3 ( 0.0f, camera->up.y - 0.01, 0.0f );
            }


            if (input->keyStates[static_cast<size_t>(Key::DOWN)]) {
                auto camera = Tools::getCamera ();
                camera->up = glm::vec3 ( 0.0f, camera->up.y - 0.01, 0.0f );
            }

            */

        }
    }
}
