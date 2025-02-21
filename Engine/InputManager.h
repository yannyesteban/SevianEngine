#pragma once
#include <IInputManager.h>
#include <glm/glm.hpp> 
#include <bitset>

namespace SEVIAN {
	class InputManager :
		public INPUT::IInputManager
	{
	private:
		std::bitset<static_cast<size_t>(INPUT::Key::MAX_KEYS)> keyStates;
		glm::vec2 mousePosition;
		glm::vec2 mouseDelta;
		bool mouseButtons[3] = { false }; // Left, Right, Middle
		//std::vector<std::function<void ( const T& )>> subscribers;

	public:
		void handleKeyEvent ( INPUT::Key key, INPUT::KeyAction action, int mods ) override;
		void handleMouseMoveEvent ( double x, double y ) override;
		void handleMouseButtonEvent ( int button, INPUT::KeyAction action, int mods ) override;
		bool isKeyPressed ( INPUT::Key key ) const override;
	};

}

