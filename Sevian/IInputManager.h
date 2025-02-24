#pragma once
#include <glm/glm.hpp> 
#include <unordered_map>
#include <functional>

namespace SEVIAN::INPUT {
	

	
	enum class InputMode
	{
		GAMEPLAY, MENU, DIALOGUE
	};

	enum class KeyAction
	{
		PRESS,
		RELEASE
	};
	

	enum class Key
	{
		UNKNOWN = -1,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
		
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
		N0, N1, N2, N3, N4, N5, N6, N7, N8, N9,

		NP_DECIMAL,
		NP_DIVIDE,
		NP_MULTIPLY,
		NP_SUBTRACT,
		NP_ADD,
		NP_EQUAL,
		NP_ENTER,

		// Function keys
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,

		// Control keys
		SPACE,
		TAB,
		ENTER,
		ESCAPE,
		BACKSPACE,
		INSERT,
		DEL,
		RIGHT,
		LEFT,
		DOWN,
		UP,
		PAGE_UP,
		PAGE_DOWN,
		HOME,
		END,
		CAPS_LOCK,
		SCROLL_LOCK,
		NUM_LOCK,
		PRINT_SCREEN,
		PAUSE,

		// Symbol keys (adjust based on your needs and keyboard layouts)
		SHIFT_LEFT,
		SHIFT_RIGHT,
		CONTROL_LEFT,
		CONTROL_RIGHT,
		ALT_LEFT,
		ALT_RIGHT,
		SUPER_LEFT,
		SUPER_RIGHT,
		MENU,

		MINUS, // For KP_SUBTRACT and regular minus
		PLUS,  // For KP_ADD and regular plus
		EQUAL,
		COMMA,
		PERIOD,
		SLASH,
		BACKSLASH,
		SEMICOLON,
		APOSTROPHE,
		GRAVE_ACCENT,
		BRACKET_LEFT,
		BRACKET_RIGHT,
		WORLD_1,
		WORLD_2,

		// Mouse Buttons
		MOUSE_BUTTON_LEFT,
		MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_MIDDLE,
		MOUSE_BUTTON_4,
		MOUSE_BUTTON_5,
		MOUSE_BUTTON_6,
		MOUSE_BUTTON_7,
		MOUSE_BUTTON_8,

		MAX_KEYS // Esto asegura que tengamos un tamaño correcto para el bitset
	};

	class IInputManager

	{
	private:
		// Callbacks para eventos de teclado
		std::vector<std::function<void ( INPUT::Key, INPUT::KeyAction, int )>> keySubscribers;
	public:
		virtual void handleKeyEvent ( Key key, KeyAction action, int mods ) = 0;
		virtual void handleMouseMoveEvent ( double x, double y ) = 0;
		virtual void handleMouseButtonEvent ( int button, KeyAction action, int mods ) = 0;

		virtual bool isKeyPressed ( Key key ) const = 0;
		virtual void subscribeKeyInput ( std::function<void ( INPUT::Key, INPUT::KeyAction, int )> ) = 0;
		virtual void subscribeMouseButtonInput ( std::function<void ( int, INPUT::KeyAction, int )> ) = 0;
		virtual void subscribeMouseMoveInput ( std::function<void ( double, double )> ) = 0;

	};


}