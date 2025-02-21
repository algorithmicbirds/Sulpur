#include "app.hpp"
#include <vulkan/vulkan_core.h>
#include <glm/ext/vector_float2.hpp>
#include "sulpur_pipeline.hpp"

#define GLM_FORCE_RADIANS
#define GLM_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <glm/gtc/constants.hpp>
#include <chrono>
#include <thread>
#include <array>

namespace Sulpur
{

struct SimplePushConstantData
{

  glm::mat2 transform{1.0f};
  glm::vec2 offset;
  alignas(16) glm::vec3 color;
};

} // namespace Sulpur

Sulpur::App::App()
{
  loadGameObjects();
  createPipelineLayout();
  recreateSwapChain();
  createCommandBuffers();
}

Sulpur::App::~App()
{
  vkDestroyPipelineLayout(sulpurDevice.device(), pipelineLayout, nullptr);
}

void Sulpur::App::run()
{
  while (!sulpurWindow.shouldClose())
  {
    glfwPollEvents();
    drawFrame();
  }
}

void Sulpur::App::loadGameObjects()
{
  std::vector<SulpurModel::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f},  {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
  };

  std::vector<glm::vec3> colors{
      {0.1f, 0.4f, 0.8f},
      {0.2f, 0.6f, 1.f },
      {0.3f, 0.7f, 1.f },
      {0.2f, 0.4f, 0.9f},
      {0.0f, 0.5f, 0.8f},
  };
  for (auto &color : colors)
  {
    color = glm::pow(color, glm::vec3{2.2f});
  }

  auto sulpurModel = std::make_shared<SulpurModel>(sulpurDevice, vertices);
  for (int i = 0; i < 40; i++)
  {
    auto triangle = SulpurGameObject::createGameObject();
    triangle.model = sulpurModel;
    triangle.transform2d.scale = glm::vec2(.5f) + i * 0.025f;
    triangle.transform2d.rotation = i * glm::two_pi<float>() * .025f;
    triangle.color = colors[i % colors.size()];
    gameObjects.push_back(std::move(triangle));
  }
}

void Sulpur::App::createPipelineLayout()
{

  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(SimplePushConstantData);

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
  if (vkCreatePipelineLayout(
          sulpurDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout
      ) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create pipeline layout");
  }
}

void Sulpur::App::createPipeline()
{
  assert(
      pipelineLayout != nullptr &&
      "Cannot create pipeline before pipeline layout"
  );
  assert(
      sulpurSwapChain != nullptr && "Cannot create pipeline before swap chain"
  );

  PipelineConfigInfo pipelineConfig{};
  SulpurPipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = sulpurSwapChain->getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  sulpurPipeline = std::make_unique<SulpurPipeline>(
      sulpurDevice, RESOURCE_PATH "shaders/vertex_shader.spv",
      RESOURCE_PATH "shaders/fragment_shader.spv", pipelineConfig
  );
}

void Sulpur::App::recreateSwapChain()
{
  auto extent = sulpurWindow.getExtent();
  while (extent.width == 0 || extent.height == 0)
  {
    extent = sulpurWindow.getExtent();
    glfwWaitEvents();
  }
  vkDeviceWaitIdle(sulpurDevice.device());

  if (sulpurSwapChain == nullptr)
  {
    sulpurSwapChain = std::make_unique<SulpurSwapChain>(sulpurDevice, extent);
  }
  else
  {
    sulpurSwapChain = std::make_unique<SulpurSwapChain>(
        sulpurDevice, extent, std::move(sulpurSwapChain)
    );
    if (sulpurSwapChain->imageCount() != commandBuffers.size())
    {
      freeCommandBuffers();
      createCommandBuffers();
    }
  }

  createPipeline();
}

void Sulpur::App::createCommandBuffers()
{
  commandBuffers.resize(sulpurSwapChain->imageCount());
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = sulpurDevice.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(
          sulpurDevice.device(), &allocInfo, commandBuffers.data()
      ) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void Sulpur::App::freeCommandBuffers()
{
  vkFreeCommandBuffers(
      sulpurDevice.device(), sulpurDevice.getCommandPool(),
      static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data()
  );
}

void Sulpur::App::recordCommandBuffer(int imageIndex)
{
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) !=
      VK_SUCCESS)
  {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = sulpurSwapChain->getRenderPass();
  renderPassInfo.framebuffer = sulpurSwapChain->getFrameBuffer(imageIndex);
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = sulpurSwapChain->getSwapChainExtent();
  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();
  vkCmdBeginRenderPass(
      commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE
  );

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width =
      static_cast<float>(sulpurSwapChain->getSwapChainExtent().width);
  viewport.height =
      static_cast<float>(sulpurSwapChain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = sulpurSwapChain->getSwapChainExtent();
  vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

  renderGameObject(commandBuffers[imageIndex]);

  vkCmdEndRenderPass(commandBuffers[imageIndex]);
  if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to record command buffer!");
  }
}

void Sulpur::App::renderGameObject(VkCommandBuffer commandBuffer)
{
  // Calculate delta time
  auto currentFrameTime = std::chrono::steady_clock::now();
  std::chrono::steady_clock::time_point lastFrameTime;
  std::chrono::duration<float> deltaTime = currentFrameTime - lastFrameTime;
  lastFrameTime = currentFrameTime;

  // Update
  int i = 0;
  double rotationSpeed =
      0.00000001; // Rotation speed (adjust this value to control the speed)
  for (auto &obj : gameObjects)
  {
    i += 1;
    // Adjust rotation so that the objects rotate in a 360-degree loop
    obj.transform2d.rotation = glm::mod<float>(
        obj.transform2d.rotation + rotationSpeed * deltaTime.count() * i,
        2.f * glm::pi<float>()
    );
  }

  // Render
  sulpurPipeline->bind(commandBuffer);
  for (auto &obj : gameObjects)
  {
    SimplePushConstantData push{};
    push.offset = obj.transform2d.translation;
    push.color = obj.color;
    push.transform = obj.transform2d.mat2();

    vkCmdPushConstants(
        commandBuffer, pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
        sizeof(SimplePushConstantData), &push
    );
    obj.model->bind(commandBuffer);
    obj.model->draw(commandBuffer);
  }
}

void Sulpur::App::drawFrame()
{
  uint32_t imageIndex;
  auto result = sulpurSwapChain->acquireNextImage(&imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR)
  {
    recreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
  {
    throw std::runtime_error("failed to acquire swap chain image!");
  }
  recordCommandBuffer(imageIndex);
  result = sulpurSwapChain->submitCommandBuffers(
      &commandBuffers[imageIndex], &imageIndex
  );
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      sulpurWindow.wasWindowResized())
  {
    sulpurWindow.resetWindowResizedFlag();
    recreateSwapChain();
  }
  if (result != VK_SUCCESS)
  {
    throw std::runtime_error("failed to present swap chain image!");
  }
}
