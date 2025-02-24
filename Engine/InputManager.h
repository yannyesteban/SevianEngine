#pragma once
#include <IInputManager.h>
#include <glm/glm.hpp> 
#include <bitset>
#include <functional>

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


		// Callbacks para eventos de teclado
		std::vector<std::function<void ( INPUT::Key, INPUT::KeyAction, int )>> keySubscribers;
		// Callbacks para eventos de movimiento del ratón
		std::vector<std::function<void ( double, double )>> mouseMoveSubscribers;
		// Callbacks para eventos de botones del ratón
		std::vector<std::function<void ( int, INPUT::KeyAction, int )>> mouseButtonSubscribers;

	public:
		void handleKeyEvent ( INPUT::Key key, INPUT::KeyAction action, int mods ) override;
		void handleMouseMoveEvent ( double x, double y ) override;
		void handleMouseButtonEvent ( int button, INPUT::KeyAction action, int mods ) override;
		bool isKeyPressed ( INPUT::Key key ) const override;

		void subscribeKeyInput ( std::function<void ( INPUT::Key, INPUT::KeyAction, int )> ) override;
		void subscribeMouseButtonInput ( std::function<void ( int, INPUT::KeyAction, int )> ) override;
		void subscribeMouseMoveInput ( std::function<void ( double, double )> ) override;
	};

}

