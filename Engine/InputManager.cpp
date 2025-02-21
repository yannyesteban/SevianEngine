#include "InputManager.h"
#include <iostream>

namespace SEVIAN {
	void InputManager::handleKeyEvent ( INPUT::Key key, INPUT::KeyAction action, int mods ) {
		std::cout << "Hola Yanny\n";
		if (key != INPUT::Key::UNKNOWN) {
			keyStates[static_cast<size_t>(key)] = (action != INPUT::KeyAction::RELEASE);
		}
	}
	void InputManager::handleMouseMoveEvent ( double x, double y ) {
		static glm::vec2 lastPos = { x, y };
		mouseDelta = { x - lastPos.x, y - lastPos.y };
		mousePosition = { x, y };
		lastPos = mousePosition;
	}
	void InputManager::handleMouseButtonEvent ( int button, INPUT::KeyAction action, int mods ) {

		
		if (button >= 0 && button < 3) {
			mouseButtons[button] = (action == INPUT::KeyAction::PRESS);
		}
	}
	bool InputManager::isKeyPressed ( INPUT::Key key ) const {
		return keyStates[static_cast<size_t>(key)];
	}
}
