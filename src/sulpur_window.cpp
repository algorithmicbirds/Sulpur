#include "sulpur_window.hpp"

// std
#include <stdexcept>

namespace Sulpur {

SulpurWindow::SulpurWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
    initWindow();
}

SulpurWindow::~SulpurWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void SulpurWindow::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void SulpurWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to craete window surface");
    }
}

void SulpurWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
    auto sulpurWindow = reinterpret_cast<SulpurWindow *>(glfwGetWindowUserPointer(window));
    sulpurWindow->framebufferResized = true;
    sulpurWindow->width = width;
    sulpurWindow->height = height;
}

} // namespace Sulpur
