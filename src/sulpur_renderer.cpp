#include "sulpur_renderer.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace Sulpur {

SulpurRenderer::SulpurRenderer(SulpurWindow &window, SulpurDevice &device)
    : sulpurWindow{window}, sulpurDevice{device} {
    recreateSwapChain();
    createCommandBuffers();
}

SulpurRenderer::~SulpurRenderer() { freeCommandBuffers(); }

void SulpurRenderer::recreateSwapChain() {
    auto extent = sulpurWindow.getExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = sulpurWindow.getExtent();
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(sulpurDevice.device());

    if (sulpurSwapChain == nullptr) {
        sulpurSwapChain = std::make_unique<SulpurSwapChain>(sulpurDevice, extent);
    } else {
        std::shared_ptr<SulpurSwapChain> oldSwapChain = std::move(sulpurSwapChain);
        sulpurSwapChain = std::make_unique<SulpurSwapChain>(sulpurDevice, extent, oldSwapChain);

        if (!oldSwapChain->compareSwapFormats(*sulpurSwapChain.get())) {
            throw std::runtime_error("Swap chain image(or depth) format has changed!");
        }
    }
}

void SulpurRenderer::createCommandBuffers() {
    commandBuffers.resize(SulpurSwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = sulpurDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(sulpurDevice.device(), &allocInfo, commandBuffers.data()) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void SulpurRenderer::freeCommandBuffers() {
    vkFreeCommandBuffers(
        sulpurDevice.device(), sulpurDevice.getCommandPool(),
        static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data()
    );
    commandBuffers.clear();
}

VkCommandBuffer SulpurRenderer::beginFrame() {
    assert(!isFrameStarted && "Can't call beginFrame while already in progress");

    auto result = sulpurSwapChain->acquireNextImage(&currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return nullptr;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    isFrameStarted = true;

    auto commandBuffer = getCurrentCommandBuffer();
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    return commandBuffer;
}

void SulpurRenderer::endFrame() {
    assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
    auto commandBuffer = getCurrentCommandBuffer();
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

    auto result = sulpurSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
        sulpurWindow.wasWindowResized()) {
        sulpurWindow.resetWindowResizedFlag();
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    isFrameStarted = false;
    currentFrameIndex = (currentFrameIndex + 1) % SulpurSwapChain::MAX_FRAMES_IN_FLIGHT;
}

void SulpurRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
    assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
    assert(
        commandBuffer == getCurrentCommandBuffer() &&
        "Can't begin render pass on command buffer from a different frame"
    );

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = sulpurSwapChain->getRenderPass();
    renderPassInfo.framebuffer = sulpurSwapChain->getFrameBuffer(currentImageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = sulpurSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(sulpurSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(sulpurSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{
        {0, 0},
        sulpurSwapChain->getSwapChainExtent()
    };
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void SulpurRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
    assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
    assert(
        commandBuffer == getCurrentCommandBuffer() &&
        "Can't end render pass on command buffer from a different frame"
    );
    vkCmdEndRenderPass(commandBuffer);
}

} // namespace Sulpur
