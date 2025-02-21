#include "Tools.h"
#include <iostream>
#include <bitset>

namespace SEVIAN {
	Key lastKey = Key::UNKNOWN;
	Camera* lastCamera;
	std::bitset<static_cast<size_t>(Key::MAX_KEYS)> keyStates;

	SEVIAN::INPUT::KeyAction Tools::GLFWKeyAction ( int glfwKeyAction ) {
		switch (glfwKeyAction) {
		case GLFW_PRESS: return INPUT::KeyAction::PRESS;
		case GLFW_RELEASE: return INPUT::KeyAction::RELEASE;
		}
	}

	SEVIAN::INPUT::Key Tools::GLFWKey ( int glfwKey ) {
		switch (glfwKey) {
		case GLFW_KEY_0: return INPUT::Key::K0;
		case GLFW_KEY_1: return INPUT::Key::K1;
		case GLFW_KEY_2: return INPUT::Key::K2;
		case GLFW_KEY_3: return INPUT::Key::K3;
		case GLFW_KEY_4: return INPUT::Key::K4;
		case GLFW_KEY_5: return INPUT::Key::K5;
		case GLFW_KEY_6: return INPUT::Key::K6;
		case GLFW_KEY_7: return INPUT::Key::K7;
		case GLFW_KEY_8: return INPUT::Key::K8;
		case GLFW_KEY_9: return INPUT::Key::K9;

		case GLFW_KEY_A: return INPUT::Key::A;
		case GLFW_KEY_B: return INPUT::Key::B;
		case GLFW_KEY_C: return INPUT::Key::C;
		case GLFW_KEY_D: return INPUT::Key::D;
		case GLFW_KEY_E: return INPUT::Key::E;
		case GLFW_KEY_F: return INPUT::Key::F;
		case GLFW_KEY_G: return INPUT::Key::G;
		case GLFW_KEY_H: return INPUT::Key::H;
		case GLFW_KEY_I: return INPUT::Key::I;
		case GLFW_KEY_J: return INPUT::Key::J;
		case GLFW_KEY_K: return INPUT::Key::K;
		case GLFW_KEY_L: return INPUT::Key::L;
		case GLFW_KEY_M: return INPUT::Key::M;
		case GLFW_KEY_N: return INPUT::Key::N;
		case GLFW_KEY_O: return INPUT::Key::O;
		case GLFW_KEY_P: return INPUT::Key::P;
		case GLFW_KEY_Q: return INPUT::Key::Q;
		case GLFW_KEY_R: return INPUT::Key::R;
		case GLFW_KEY_S: return INPUT::Key::S;
		case GLFW_KEY_T: return INPUT::Key::T;
		case GLFW_KEY_U: return INPUT::Key::U;
		case GLFW_KEY_V: return INPUT::Key::V;
		case GLFW_KEY_W: return INPUT::Key::W;
		case GLFW_KEY_X: return INPUT::Key::X;
		case GLFW_KEY_Y: return INPUT::Key::Y;
		case GLFW_KEY_Z: return INPUT::Key::Z;

		case GLFW_KEY_KP_SUBTRACT: return INPUT::Key::MINUS; // Or Key::NP_SUBTRACT if you want to differentiate
		case GLFW_KEY_KP_ADD: return INPUT::Key::PLUS;      // Or Key::NP_ADD
		case GLFW_KEY_RIGHT:    return INPUT::Key::RIGHT;
		case GLFW_KEY_LEFT:     return INPUT::Key::LEFT;
		case GLFW_KEY_DOWN:     return INPUT::Key::DOWN;
		case GLFW_KEY_UP:       return INPUT::Key::UP;

		case GLFW_KEY_KP_0: return INPUT::Key::N0;
		case GLFW_KEY_KP_1: return INPUT::Key::N1;
		case GLFW_KEY_KP_2: return INPUT::Key::N2;
		case GLFW_KEY_KP_3: return INPUT::Key::N3;
		case GLFW_KEY_KP_4: return INPUT::Key::N4;
		case GLFW_KEY_KP_5: return INPUT::Key::N5;
		case GLFW_KEY_KP_6: return INPUT::Key::N6;
		case GLFW_KEY_KP_7: return INPUT::Key::N7;
		case GLFW_KEY_KP_8: return INPUT::Key::N8;
		case GLFW_KEY_KP_9: return INPUT::Key::N9;

		case GLFW_MOUSE_BUTTON_LEFT:   return INPUT::Key::MOUSE_BUTTON_LEFT;
		case GLFW_MOUSE_BUTTON_RIGHT:  return INPUT::Key::MOUSE_BUTTON_RIGHT;
		case GLFW_MOUSE_BUTTON_MIDDLE: return INPUT::Key::MOUSE_BUTTON_MIDDLE;
		case GLFW_MOUSE_BUTTON_4:      return INPUT::Key::MOUSE_BUTTON_4;
		case GLFW_MOUSE_BUTTON_5:      return INPUT::Key::MOUSE_BUTTON_5;
		case GLFW_MOUSE_BUTTON_6:      return INPUT::Key::MOUSE_BUTTON_6;
		case GLFW_MOUSE_BUTTON_7:      return INPUT::Key::MOUSE_BUTTON_7;
		case GLFW_MOUSE_BUTTON_8:      return INPUT::Key::MOUSE_BUTTON_8;


		default: return INPUT::Key::UNKNOWN;

		}
	}
	
	void Tools::setCamera ( Camera* camera ) {
		if (camera != nullptr) {
			lastCamera = camera;
		}
		else {
			std::cerr << "Error: Intento de asignar un puntero nulo a la cámara." << std::endl;
		}
		std::cout << "...lastCamera set " << camera->position.z << "\n";
	}

	Camera* Tools::getCamera () {
		std::cout << "...lastCamera get " << lastCamera->position.z << "\n";
		return lastCamera;
	}

	bool Tools::isKeyPressed ( Key key ) {
		//std::cout << static_cast<size_t>(key) << "\n";
		return keyStates[static_cast<size_t>(key)];
		/*
		if (lastKey == key) {
			return true;
		}
		return false;*/
	}

	void Tools::setLastKey ( Key key ) {
		lastKey = key;
	}
	void Tools::keyCallback2 ( GLFWwindow* window, int _key, int scancode, int _action, int mods ) {

		auto key = Tools::GLFWKey ( _key );
		auto action = Tools::GLFWKeyAction ( _action );

		SEVIAN::INPUT::IInputManager* game = static_cast<SEVIAN::INPUT::IInputManager*>(glfwGetWindowUserPointer ( window ));
		if (game) {
			game->handleKeyEvent ( key, action, mods );
		}

	}

	void Tools::CursorCallback ( GLFWwindow* window, double xpos, double ypos ) {
		SEVIAN::INPUT::IInputManager* game = static_cast<SEVIAN::INPUT::IInputManager*>(glfwGetWindowUserPointer ( window ));

		std::cout << "Mouse move " << xpos << " " << ypos << "\n";
		if (game) {
			game->handleMouseMoveEvent ( xpos, ypos );
		}
	}

	void Tools::MouseButtonCallback ( GLFWwindow* window, int button, int action, int mods ) {

	}

	void Tools::ScrollCallback ( GLFWwindow* window, double xoffset, double yoffset ) {
	}


	void Tools::keyCallback ( GLFWwindow* window, int key, int scancode, int action, int mods ) {




		if (action == GLFW_PRESS) {
			bool value = true;
			switch (key) {


			case GLFW_KEY_0:
				keyStates[static_cast<size_t>(Key::K0)] = value;
				break;
			case GLFW_KEY_1:
				keyStates[static_cast<size_t>(Key::K1)] = value;
				break;
			case GLFW_KEY_2:
				keyStates[static_cast<size_t>(Key::K2)] = value;
				break;
			case GLFW_KEY_3:
				keyStates[static_cast<size_t>(Key::K3)] = value;
				break;
			case GLFW_KEY_4:
				keyStates[static_cast<size_t>(Key::K4)] = value;
				break;
			case GLFW_KEY_5:
				keyStates[static_cast<size_t>(Key::K5)] = value;
				break;
			case GLFW_KEY_6:
				keyStates[static_cast<size_t>(Key::K6)] = value;
				break;
			case GLFW_KEY_7:
				keyStates[static_cast<size_t>(Key::K7)] = value;
				break;
			case GLFW_KEY_8:
				keyStates[static_cast<size_t>(Key::K8)] = value;
				break;
			case GLFW_KEY_9:
				keyStates[static_cast<size_t>(Key::K9)] = value;
				break;




			case GLFW_KEY_A:
				keyStates[static_cast<size_t>(Key::A)] = true;
				break;
			case GLFW_KEY_B:
				keyStates[static_cast<size_t>(Key::B)] = true;
				break;
			case GLFW_KEY_C:
				keyStates[static_cast<size_t>(Key::C)] = true;
				break;
			case GLFW_KEY_D:
				keyStates[static_cast<size_t>(Key::D)] = true;
				break;
			case GLFW_KEY_E:
				keyStates[static_cast<size_t>(Key::E)] = true;
				break;
			case GLFW_KEY_F:
				keyStates[static_cast<size_t>(Key::F)] = true;
				break;
			case GLFW_KEY_G:
				keyStates[static_cast<size_t>(Key::G)] = true;
				break;
			case GLFW_KEY_H:
				keyStates[static_cast<size_t>(Key::H)] = true;
				break;
			case GLFW_KEY_I:
				keyStates[static_cast<size_t>(Key::I)] = true;
				break;
			case GLFW_KEY_J:
				keyStates[static_cast<size_t>(Key::J)] = true;
				break;
			case GLFW_KEY_K:
				keyStates[static_cast<size_t>(Key::K)] = true;
				break;
			case GLFW_KEY_L:
				keyStates[static_cast<size_t>(Key::L)] = true;
				break;
			case GLFW_KEY_M:
				keyStates[static_cast<size_t>(Key::M)] = true;
				break;
			case GLFW_KEY_N:
				keyStates[static_cast<size_t>(Key::N)] = true;
				break;
			case GLFW_KEY_O:
				keyStates[static_cast<size_t>(Key::O)] = true;
				break;
			case GLFW_KEY_P:
				keyStates[static_cast<size_t>(Key::P)] = true;
				break;
			case GLFW_KEY_Q:
				keyStates[static_cast<size_t>(Key::Q)] = true;
				break;
			case GLFW_KEY_R:
				keyStates[static_cast<size_t>(Key::R)] = true;
				break;
			case GLFW_KEY_S:
				keyStates[static_cast<size_t>(Key::S)] = true;
				break;
			case GLFW_KEY_T:
				keyStates[static_cast<size_t>(Key::T)] = true;
				break;
			case GLFW_KEY_U:
				keyStates[static_cast<size_t>(Key::U)] = true;
				break;
			case GLFW_KEY_V:
				keyStates[static_cast<size_t>(Key::V)] = true;
				break;
			case GLFW_KEY_W:
				keyStates[static_cast<size_t>(Key::W)] = true;
				break;
			case GLFW_KEY_X:
				keyStates[static_cast<size_t>(Key::X)] = true;
				break;
			case GLFW_KEY_Y:
				keyStates[static_cast<size_t>(Key::Y)] = true;
				break;
			case GLFW_KEY_Z:
				keyStates[static_cast<size_t>(Key::Z)] = true;
				break;


			case GLFW_KEY_KP_SUBTRACT:
				keyStates[static_cast<size_t>(Key::MINUS)] = true;
				break;

			case GLFW_KEY_KP_ADD:
				keyStates[static_cast<size_t>(Key::PLUS)] = true;
				break;
			case GLFW_KEY_RIGHT:
				keyStates[static_cast<size_t>(Key::RIGHT)] = true;

				break;
			case GLFW_KEY_LEFT:
				keyStates[static_cast<size_t>(Key::LEFT)] = true;
				break;
			case GLFW_KEY_DOWN:
				keyStates[static_cast<size_t>(Key::DOWN)] = true;
				break;
			case GLFW_KEY_UP:
				keyStates[static_cast<size_t>(Key::UP)] = true;
				break;

			case GLFW_KEY_KP_0:

				keyStates[static_cast<size_t>(Key::N0)] = true;
				break;

			case GLFW_KEY_KP_1:

				keyStates[static_cast<size_t>(Key::N1)] = true;
				break;

			case GLFW_KEY_KP_2:
				keyStates[static_cast<size_t>(Key::N2)] = true;
				break;
			case GLFW_KEY_KP_3:

				keyStates[static_cast<size_t>(Key::N3)] = true;
				break;

			case GLFW_KEY_KP_4:
				keyStates[static_cast<size_t>(Key::N4)] = true;
				break;
			case GLFW_KEY_KP_5:
				keyStates[static_cast<size_t>(Key::N5)] = true;
				break;
			case GLFW_KEY_KP_6:

				keyStates[static_cast<size_t>(Key::N6)] = true;
				break;
			case GLFW_KEY_KP_7:

				keyStates[static_cast<size_t>(Key::N7)] = true;
				break;
			case GLFW_KEY_KP_8:

				keyStates[static_cast<size_t>(Key::N8)] = true;
				break;
			case GLFW_KEY_KP_9:

				keyStates[static_cast<size_t>(Key::N9)] = true;
				break;
			}



		}


		else if (action == GLFW_RELEASE) {

			bool value = false;


			switch (key) {
			case GLFW_KEY_0:
				keyStates[static_cast<size_t>(Key::K0)] = value;
				break;
			case GLFW_KEY_1:
				keyStates[static_cast<size_t>(Key::K1)] = value;
				break;
			case GLFW_KEY_2:
				keyStates[static_cast<size_t>(Key::K2)] = value;
				break;
			case GLFW_KEY_3:
				keyStates[static_cast<size_t>(Key::K3)] = value;
				break;
			case GLFW_KEY_4:
				keyStates[static_cast<size_t>(Key::K4)] = value;
				break;
			case GLFW_KEY_5:
				keyStates[static_cast<size_t>(Key::K5)] = value;
				break;
			case GLFW_KEY_6:
				keyStates[static_cast<size_t>(Key::K6)] = value;
				break;
			case GLFW_KEY_7:
				keyStates[static_cast<size_t>(Key::K7)] = value;
				break;
			case GLFW_KEY_8:
				keyStates[static_cast<size_t>(Key::K8)] = value;
				break;
			case GLFW_KEY_9:
				keyStates[static_cast<size_t>(Key::K9)] = value;
				break;

			case GLFW_KEY_A:
				keyStates[static_cast<size_t>(Key::A)] = false;
				break;
			case GLFW_KEY_B:
				keyStates[static_cast<size_t>(Key::B)] = false;
				break;
			case GLFW_KEY_C:
				keyStates[static_cast<size_t>(Key::C)] = false;
				break;
			case GLFW_KEY_D:
				keyStates[static_cast<size_t>(Key::D)] = false;
				break;
			case GLFW_KEY_E:
				keyStates[static_cast<size_t>(Key::E)] = false;
				break;
			case GLFW_KEY_F:
				keyStates[static_cast<size_t>(Key::F)] = false;
				break;
			case GLFW_KEY_G:
				keyStates[static_cast<size_t>(Key::G)] = false;
				break;
			case GLFW_KEY_H:
				keyStates[static_cast<size_t>(Key::H)] = false;
				break;
			case GLFW_KEY_I:
				keyStates[static_cast<size_t>(Key::I)] = false;
				break;
			case GLFW_KEY_J:
				keyStates[static_cast<size_t>(Key::J)] = false;
				break;
			case GLFW_KEY_K:
				keyStates[static_cast<size_t>(Key::K)] = false;
				break;
			case GLFW_KEY_L:
				keyStates[static_cast<size_t>(Key::L)] = false;
				break;
			case GLFW_KEY_M:
				keyStates[static_cast<size_t>(Key::M)] = false;
				break;
			case GLFW_KEY_N:
				keyStates[static_cast<size_t>(Key::N)] = false;
				break;
			case GLFW_KEY_O:
				keyStates[static_cast<size_t>(Key::O)] = false;
				break;
			case GLFW_KEY_P:
				keyStates[static_cast<size_t>(Key::P)] = false;
				break;
			case GLFW_KEY_Q:
				keyStates[static_cast<size_t>(Key::Q)] = false;
				break;
			case GLFW_KEY_R:
				keyStates[static_cast<size_t>(Key::R)] = false;
				break;
			case GLFW_KEY_S:
				keyStates[static_cast<size_t>(Key::S)] = false;
				break;
			case GLFW_KEY_T:
				keyStates[static_cast<size_t>(Key::T)] = false;
				break;
			case GLFW_KEY_U:
				keyStates[static_cast<size_t>(Key::U)] = false;
				break;
			case GLFW_KEY_V:
				keyStates[static_cast<size_t>(Key::V)] = false;
				break;
			case GLFW_KEY_W:
				keyStates[static_cast<size_t>(Key::W)] = false;
				break;
			case GLFW_KEY_X:
				keyStates[static_cast<size_t>(Key::X)] = false;
				break;
			case GLFW_KEY_Y:
				keyStates[static_cast<size_t>(Key::Y)] = false;
				break;
			case GLFW_KEY_Z:
				keyStates[static_cast<size_t>(Key::Z)] = false;
				break;

			case GLFW_KEY_KP_SUBTRACT:
				keyStates[static_cast<size_t>(Key::MINUS)] = false;
				break;

			case GLFW_KEY_KP_ADD:
				keyStates[static_cast<size_t>(Key::PLUS)] = false;
				break;

			case GLFW_KEY_RIGHT:
				keyStates[static_cast<size_t>(Key::RIGHT)] = false;

				break;
			case GLFW_KEY_LEFT:
				keyStates[static_cast<size_t>(Key::LEFT)] = false;
				break;
			case GLFW_KEY_DOWN:
				keyStates[static_cast<size_t>(Key::DOWN)] = false;
				break;
			case GLFW_KEY_UP:
				keyStates[static_cast<size_t>(Key::UP)] = false;
				break;
			case GLFW_KEY_KP_0:
				keyStates[static_cast<size_t>(Key::N0)] = false;
				break;
			case GLFW_KEY_KP_1:
				keyStates[static_cast<size_t>(Key::N1)] = false;
				break;
			case GLFW_KEY_KP_2:
				keyStates[static_cast<size_t>(Key::N2)] = false;
				break;
			case GLFW_KEY_KP_3:
				keyStates[static_cast<size_t>(Key::N3)] = false;
				break;
			case GLFW_KEY_KP_4:
				keyStates[static_cast<size_t>(Key::N4)] = false;
				break;
			case GLFW_KEY_KP_5:
				keyStates[static_cast<size_t>(Key::N5)] = false;
				break;
			case GLFW_KEY_KP_6:
				keyStates[static_cast<size_t>(Key::N6)] = false;
				break;
			case GLFW_KEY_KP_7:
				keyStates[static_cast<size_t>(Key::N7)] = false;
				break;
			case GLFW_KEY_KP_8:
				keyStates[static_cast<size_t>(Key::N8)] = false;
				break;
			case GLFW_KEY_KP_9:
				keyStates[static_cast<size_t>(Key::N9)] = false;
				break;
			}
		}
	}

}