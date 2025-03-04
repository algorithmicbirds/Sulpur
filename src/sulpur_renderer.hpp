#pragma once

#include "sulpur_device.hpp"
#include "sulpur_swap_chain.hpp"
#include "sulpur_window.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace Sulpur {
class SulpurRenderer {
  public:
    SulpurRenderer(SulpurWindow &window, SulpurDevice &device);
    ~SulpurRenderer();

    SulpurRenderer(const SulpurRenderer &) = delete;
    SulpurRenderer &operator=(const SulpurRenderer &) = delete;

    VkRenderPass getSwapChainRenderPass() const { return sulpurSwapChain->getRenderPass(); }
    float getAspectRatio() const { return sulpurSwapChain->extentAspectRatio(); }
    bool isFrameInProgress() const { return isFrameStarted; }

    VkCommandBuffer getCurrentCommandBuffer() const {
        assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
        return commandBuffers[currentFrameIndex];
    }

    int getFrameIndex() const {
        assert(isFrameStarted && "Cannot get frame index when frame not in progress");
        return currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

  private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    SulpurWindow &sulpurWindow;
    SulpurDevice &sulpurDevice;
    std::unique_ptr<SulpurSwapChain> sulpurSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIndex;
    int currentFrameIndex{0};
    bool isFrameStarted{false};
};
} // namespace Sulpur
