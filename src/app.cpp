#include "app.hpp"



void Sulpur::App::run()
{
	while (!sulpurWindow.shouldClose()) {
		glfwPollEvents();
	}
}
