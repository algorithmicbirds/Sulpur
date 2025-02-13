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

    SulpurWindow(const SulpurWindow&) = delete;
    SulpurWindow& operator=(const SulpurWindow&) = delete;

    bool shouldClose() { return glfwWindowShouldClose(window); }
    VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

   private:
    void InitWindow();

    GLFWwindow* window;
    const int height;
    const int width;
    std::string window_name;
};
}  // namespace Sulpur