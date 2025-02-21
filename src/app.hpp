#pragma once

#include "sulpur_window.hpp"
#include "sulpur_pipeline.hpp"
#include "sulpur_device.hpp"
#include "sulpur_swap_chain.hpp"
#include "sulpur_model.hpp"
#include "sulpur_game_object.hpp"
#include <memory>
#include <vector>

namespace Sulpur
{
class App
{
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  App();
  ~App();

  App(const App &) = delete;
  App &operator=(const App &) = delete;

  void run();

private:
  void loadGameObjects();
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void freeCommandBuffers();
  void drawFrame();
  void recreateSwapChain();
  void recordCommandBuffer(int imageIndex);
  void renderGameObject(VkCommandBuffer commandBuffer);

  Sulpur::SulpurWindow sulpurWindow{WIDTH, HEIGHT, "Sulpur"};
  SulpurDevice sulpurDevice{sulpurWindow};
  std::unique_ptr<SulpurSwapChain> sulpurSwapChain;
  std::unique_ptr<SulpurPipeline> sulpurPipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;

  std::vector<SulpurGameObject> gameObjects;
  ;
};
} // namespace Sulpur
