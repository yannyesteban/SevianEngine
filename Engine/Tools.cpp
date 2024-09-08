#include "Tools.h"
#include <iostream>
#include <bitset>

namespace SEVIAN {
	Key lastKey = Key::NONE;
	Camera * lastCamera;
	std::bitset<static_cast<size_t>(Key::MAX_KEYS)> keyStates;

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

	

	void Tools::keyCallback ( GLFWwindow* window, int key, int scancode, int action, int mods ) {

		/*
		keyStates[static_cast<size_t>(Key::A)] = isKeyPressed ( Key::A );
		keyStates[static_cast<size_t>(Key::B)] = isKeyPressed ( Key::B );
		keyStates[static_cast<size_t>(Key::C)] = isKeyPressed ( Key::C );
		keyStates[static_cast<size_t>(Key::D)] = isKeyPressed ( Key::D );
		keyStates[static_cast<size_t>(Key::E)] = isKeyPressed ( Key::E );
		keyStates[static_cast<size_t>(Key::F)] = isKeyPressed ( Key::F );
		keyStates[static_cast<size_t>(Key::G)] = isKeyPressed ( Key::G );
		keyStates[static_cast<size_t>(Key::H)] = isKeyPressed ( Key::H );
		keyStates[static_cast<size_t>(Key::I)] = isKeyPressed ( Key::I );
		keyStates[static_cast<size_t>(Key::J)] = isKeyPressed ( Key::J );
		keyStates[static_cast<size_t>(Key::K)] = isKeyPressed ( Key::K );
		keyStates[static_cast<size_t>(Key::L)] = isKeyPressed ( Key::L );
		keyStates[static_cast<size_t>(Key::M)] = isKeyPressed ( Key::M );
		keyStates[static_cast<size_t>(Key::N)] = isKeyPressed ( Key::N );
		keyStates[static_cast<size_t>(Key::O)] = isKeyPressed ( Key::O );
		keyStates[static_cast<size_t>(Key::P)] = isKeyPressed ( Key::P );
		keyStates[static_cast<size_t>(Key::Q)] = isKeyPressed ( Key::Q );
		keyStates[static_cast<size_t>(Key::R)] = isKeyPressed ( Key::R );
		keyStates[static_cast<size_t>(Key::S)] = isKeyPressed ( Key::S );
		keyStates[static_cast<size_t>(Key::T)] = isKeyPressed ( Key::T );
		keyStates[static_cast<size_t>(Key::U)] = isKeyPressed ( Key::U );
		keyStates[static_cast<size_t>(Key::V)] = isKeyPressed ( Key::V );
		keyStates[static_cast<size_t>(Key::W)] = isKeyPressed ( Key::W );
		keyStates[static_cast<size_t>(Key::X)] = isKeyPressed ( Key::X );
		keyStates[static_cast<size_t>(Key::Y)] = isKeyPressed ( Key::Y );
		keyStates[static_cast<size_t>(Key::Z)] = isKeyPressed ( Key::Z );
		keyStates[static_cast<size_t>(Key::LEFT)] = isKeyPressed ( Key::A );
		keyStates[static_cast<size_t>(Key::RIGHT)] = isKeyPressed ( Key::A );
		keyStates[static_cast<size_t>(Key::UP)] = isKeyPressed ( Key::A );
		keyStates[static_cast<size_t>(Key::DOWN)] = isKeyPressed ( Key::A );
		*/
		//std::cout << keyStates[static_cast<size_t>(Key::A)] << "..\n";


		if (action == GLFW_PRESS) {
			switch (key) {
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
			


			switch (key) {
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