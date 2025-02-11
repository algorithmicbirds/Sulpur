#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <string>
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Sulpur {
class SulpurWindow {
   public:
    SulpurWindow(int width, int height, std::string name);
    ~SulpurWindow();
    bool shouldClose() { return glfwWindowShouldClose(window); }
    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

   private:
    void InitWindow();

    GLFWwindow* window;
    const int height;
    const int width;
    std::string window_name;
};
}  // namespace Sulpur