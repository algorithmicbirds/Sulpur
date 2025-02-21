#include "sulpur_window.hpp"

namespace Sulpur
{

SulpurWindow::SulpurWindow(int w, int h, std::string name)
    : width{w}, height{h}, window_name{name}
{
  InitWindow();
}

SulpurWindow::~SulpurWindow()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}
void SulpurWindow::createWindowSurface(
    VkInstance instance, VkSurfaceKHR *surface
)
{
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create window surface");
  }
}
void SulpurWindow::framebufferResizeCallback(
    GLFWwindow *window, int width, int height
)
{
  auto sulpurWindow =
      reinterpret_cast<SulpurWindow *>(glfwGetWindowUserPointer(window));
  sulpurWindow->frameBufferResized = true;
  sulpurWindow->width = width;
  sulpurWindow->height = height;

  // std::cout << "framebufferResizeCallback: " << width << " " << height <<
  // std::endl;
}
void SulpurWindow::InitWindow()
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  window =
      glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

} // namespace Sulpur
