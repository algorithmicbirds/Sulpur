#pragma once

#include "sulpur_window.hpp"
#include "sulpur_device.hpp"
#include "sulpur_swap_chain.hpp"
#include <algorithm>
#include <memory>
#include <vector>
#include <cassert>
#include <iostream>
namespace Sulpur {
class SulpurRenderer {
  public:
    SulpurRenderer(SulpurWindow &window, SulpurDevice &device);
    ~SulpurRenderer();

    SulpurRenderer(const SulpurRenderer &) = delete;
    SulpurRenderer &operator=(const SulpurRenderer &) = delete;

    bool isFrameInProgress() const { return isFrameStarted; }

    VkRenderPass getSwapChainRenderPass() const { return sulpurSwapChain->getRenderPass(); }
    VkCommandBuffer getCurrentCommandBuffer() const {
        assert(isFrameStarted && "Cannot get command buffer when frame is not in progress");
        assert(
            currentFrameIndex < commandBuffers.size() &&
            "currentFrameIndex exceeds commandBuffers size"
        );

        return commandBuffers[currentFrameIndex];
    }

    int getFrameIndex() const {
        assert(isFrameStarted && "cannot get frame index when frame is bit in progress");
        return currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

  private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void drawFrame();
    void recreateSwapChain();

    SulpurWindow &sulpurWindow;
    SulpurDevice &sulpurDevice;
    std::unique_ptr<SulpurSwapChain> sulpurSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIndex;
    bool isFrameStarted{false};
    int currentFrameIndex = 0;
};
} // namespace Sulpur
