#include "InputSystem.h"


void SEVIAN::InputSystem::init ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) {
}

void SEVIAN::InputSystem::update ( std::vector<std::shared_ptr<Entity>>& entities, float deltaTime ) {

    for (auto& entity : entities) {
        auto input = entity->getComponent<InputComponent> ();
       

        if (input) {

            input->keyStates[static_cast<size_t>(Key::A)] = Tools::isKeyPressed ( Key::A );
            input->keyStates[static_cast<size_t>(Key::S)] = Tools::isKeyPressed ( Key::S );

            input->keyStates[static_cast<size_t>(Key::LEFT)] = Tools::isKeyPressed ( Key::LEFT );
            input->keyStates[static_cast<size_t>(Key::RIGHT)] = Tools::isKeyPressed ( Key::RIGHT );

            input->keyStates[static_cast<size_t>(Key::UP)] = Tools::isKeyPressed ( Key::UP );
            input->keyStates[static_cast<size_t>(Key::DOWN)] = Tools::isKeyPressed ( Key::DOWN );
        }
    }
}
