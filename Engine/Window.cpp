#include "Window.h"
namespace SEVIAN {
	Window::Window ( uint32_t pWidth, uint32_t pHeight, std::string pTitle ) {

		width = pWidth;
		height = pWidth;
		title = pTitle;
		glfwInit ();

		glfwWindowHint ( GLFW_CLIENT_API, GLFW_NO_API );

		window = glfwCreateWindow ( width, height, title.c_str (), nullptr, nullptr );
		glfwSetWindowUserPointer ( window, this );
		//glfwSetFramebufferSizeCallback ( window, this->resizeCallback );

		//glfwSetKeyCallback ( window, keyCallback );
	}

	GLFWwindow* Window::getWindow () {
		return window;
	}

}
