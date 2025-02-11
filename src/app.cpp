#include "app.hpp"



void Sulpur::App::run()
{
	while (!sulpur_window.shouldClose()) {
		glfwPollEvents();
	}
}
