#pragma once

#include <GLFW/glfw3.h>
#include <string>

namespace SEVIAN {
	class Window
	{
	private:
		uint32_t width;
		uint32_t height;
		std::string title;

		GLFWwindow* window;
	public:
		Window ( uint32_t pWidth, uint32_t pHeight, std::string title );

		GLFWwindow* getWindow ();
		
	};

}

