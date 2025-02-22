#pragma once

#include "sulpur_pipeline.hpp"
#include "sulpur_device.hpp"
#include "sulpur_game_object.hpp"
#include <memory>
#include <vector>

namespace Sulpur {
class SulpurRendererSystem {
  public:
    SulpurRendererSystem(SulpurDevice &device, VkRenderPass renderPass);
    ~SulpurRendererSystem();

    SulpurRendererSystem(const SulpurRendererSystem &) = delete;
    SulpurRendererSystem &operator=(const SulpurRendererSystem &) = delete;

    void run();
    void
    renderGameObjects(VkCommandBuffer commandBuffer, std::vector<SulpurGameObject> &gameObjects);

  private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);
    SulpurDevice &sulpurDevice;
    std::unique_ptr<SulpurPipeline> sulpurPipeline;
    VkPipelineLayout pipelineLayout;
};
} // namespace Sulpur
