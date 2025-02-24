#include "InputManager.h"
#include <iostream>

namespace SEVIAN {
	void InputManager::handleKeyEvent ( INPUT::Key key, INPUT::KeyAction action, int mods ) {
		std::cout << "Hola Yanny\n";
		if (key != INPUT::Key::UNKNOWN) {
			keyStates[static_cast<size_t>(key)] = (action != INPUT::KeyAction::RELEASE);
		}


		for (auto& callback : keySubscribers) {

			callback ( key, action, mods );
		}
	}
	void InputManager::handleMouseMoveEvent ( double x, double y ) {
		static glm::vec2 lastPos = { x, y };
		mouseDelta = { x - lastPos.x, y - lastPos.y };
		mousePosition = { x, y };
		lastPos = mousePosition;
		for (auto& callback : mouseMoveSubscribers) {

			callback ( x, y );
		}
	}
	void InputManager::handleMouseButtonEvent ( int button, INPUT::KeyAction action, int mods ) {

		
		if (button >= 0 && button < 3) {
			mouseButtons[button] = (action == INPUT::KeyAction::PRESS);
		}

		for (auto& callback : mouseButtonSubscribers) {

			callback ( button, action, mods );
		}
	}
	bool InputManager::isKeyPressed ( INPUT::Key key ) const {
		return keyStates[static_cast<size_t>(key)];
	}
	void InputManager::subscribeKeyInput ( std::function<void ( INPUT::Key, INPUT::KeyAction, int )> callback ) {
		keySubscribers.push_back( callback );
	}
	void InputManager::subscribeMouseButtonInput ( std::function<void ( int, INPUT::KeyAction, int )> callback  ) {
		mouseButtonSubscribers.push_back ( callback );
	}
	void InputManager::subscribeMouseMoveInput ( std::function<void ( double, double )> callback  ) {
		mouseMoveSubscribers.push_back ( callback );
	}
}
