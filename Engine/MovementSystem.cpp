#include "MovementSystem.h"

void SEVIAN::MovementSystem::init ( std::vector<std::shared_ptr<Entity>>& entities ) {
}

void SEVIAN::MovementSystem::update ( std::vector<std::shared_ptr<Entity>>& entities ) {

    for (auto& entity : entities) {
        auto input  = entity->getComponent<InputComponent> ();
        auto position = entity->getComponent<PositionComponent> ();

        auto camera = entity->getComponent<CameraComponent> ();

        if (camera) {
            //camera->position =  glm::vec3 ( 0.0f, 0.0f, -8.0f );
        }
       
        if (input && position) {
            
            if (input->keyStates[static_cast<size_t>(Key::S)]) {
               
                position->position.x += 0.001;

                
            }
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
                camera->up = glm::vec3 ( 0.0f,  camera->up.y - 0.01, 0.0f );
            }


            if (input->keyStates[static_cast<size_t>(Key::DOWN)]) {
                auto camera = Tools::getCamera ();
                camera->up = glm::vec3 ( 0.0f, camera->up.y - 0.01, 0.0f );
            }

        }
    }

}
