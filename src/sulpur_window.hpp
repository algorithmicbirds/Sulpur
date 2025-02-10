#pragma once

#include <GLFW/glfw3.h>
#include <string>

namespace Sulpur {
	class SulpurWindow {
	public:
		SulpurWindow(int width, int height, std::string name);
		~SulpurWindow();
		bool shouldClose() { return glfwWindowShouldClose(window); }
	private:
		void InitWindow();
		
		GLFWwindow* window;
		const int height;
		const int width;
		std::string window_name;
		
	};
}