#pragma once

#include "sulpur_camera.hpp"
#include "sulpur_device.hpp"
#include "sulpur_game_object.hpp"
#include "sulpur_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace Sulpur {
class SulpurRenderSystem {
  public:
    SulpurRenderSystem(SulpurDevice &device, VkRenderPass renderPass);
    ~SulpurRenderSystem();

    SulpurRenderSystem(const SulpurRenderSystem &) = delete;
    SulpurRenderSystem &operator=(const SulpurRenderSystem &) = delete;

    void renderGameObjects(
        VkCommandBuffer commandBuffer, std::vector<SulpurGameObject> &gameObjects,
        const SulpurCamera &camera
    );

  private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    SulpurDevice &sulpurDevice;

    std::unique_ptr<SulpurPipeline> sulpurPipeline;
    VkPipelineLayout pipelineLayout;
};
} // namespace Sulpur
