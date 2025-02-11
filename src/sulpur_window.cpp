#include "sulpur_window.hpp"

namespace Sulpur {

    SulpurWindow::SulpurWindow(int w, int h, std::string name) : width{ w }, height{ h }, window_name{ name } {
        InitWindow();
    }

    SulpurWindow::~SulpurWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void SulpurWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
    }
    void SulpurWindow::InitWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);
    }

}

