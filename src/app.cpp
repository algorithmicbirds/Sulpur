#include "app.hpp"
#include <vulkan/vulkan_core.h>
#include <array>
#include <glm/ext/vector_float2.hpp>
#include "sulpur_pipeline.hpp"

#define GLM_FORCE_RADIANS
#define GLM_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace Sulpur {
struct SimplePushConstantData{
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
    glm::mat2 transform{1.0f};
};

}

Sulpur::App::App() {
    loadModels();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
}

Sulpur::App::~App() { vkDestroyPipelineLayout(sulpurDevice.device(), pipelineLayout, nullptr); }

void Sulpur::App::run() {
    while (!sulpurWindow.shouldClose()) {
        glfwPollEvents();
        drawFrame();
    }
}
void Sulpur::App::loadModels() {
    std::vector<SulpurModel::Vertex> vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };
    sulpurModel = std::make_unique<SulpurModel>(sulpurDevice, vertices);
}


void Sulpur::App::createPipelineLayout() {
    
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);
    
    VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(sulpurDevice.device(), &pipelineLayoutInfo, nullptr,
                               &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout");
    }

}

void Sulpur::App::createPipeline() {
    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
    assert(sulpurSwapChain != nullptr && "Cannot create pipeline before swap chain");

    
    PipelineConfigInfo pipelineConfig{};
    SulpurPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = sulpurSwapChain->getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    sulpurPipeline = std::make_unique<SulpurPipeline>(
        sulpurDevice, RESOURCE_PATH "shaders/vertex_shader.spv", RESOURCE_PATH "shaders/fragment_shader.spv", pipelineConfig
    );
}

void Sulpur::App::recreateSwapChain() {
    auto extent = sulpurWindow.getExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = sulpurWindow.getExtent();
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(sulpurDevice.device());
    
    if (sulpurSwapChain == nullptr) {
        sulpurSwapChain = std::make_unique<SulpurSwapChain>(sulpurDevice, extent);
    } else {
        sulpurSwapChain =
        std::make_unique<SulpurSwapChain>(sulpurDevice, extent, std::move(sulpurSwapChain));
        if (sulpurSwapChain->imageCount() != commandBuffers.size()) {
            freeCommandBuffers();
            createCommandBuffers();
        }
    }
    
    createPipeline();
}

void Sulpur::App::createCommandBuffers() { 
    commandBuffers.resize(sulpurSwapChain->imageCount());
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

void Sulpur::App::freeCommandBuffers() {
    vkFreeCommandBuffers(sulpurDevice.device(), sulpurDevice.getCommandPool(),
    static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    
}

void Sulpur::App::recordCommandBuffer(int imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
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
    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(sulpurSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(sulpurSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
    
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = sulpurSwapChain->getSwapChainExtent();
    vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);
    
    sulpurPipeline->bind(commandBuffers[imageIndex]);
    sulpurModel->bind(commandBuffers[imageIndex]);

    
        for(int j = 0; j < 4; j++) {
            SimplePushConstantData push{};
            push.offset = {0.0f, -0.4f + j * 0.3f};
            push.color = {0.0f, 0.0f, 0.2f + 0.2f *j};
    
            vkCmdPushConstants(commandBuffers[imageIndex], pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT , 
                0, sizeof(SimplePushConstantData), &push);
                sulpurModel->draw(commandBuffers[imageIndex]);

        }

        sulpurModel->draw(commandBuffers[imageIndex]);

    vkCmdEndRenderPass(commandBuffers[imageIndex]);
    if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void Sulpur::App::drawFrame() {
    uint32_t imageIndex;
    auto result = sulpurSwapChain->acquireNextImage(&imageIndex);
    
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    }
    
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }
    recordCommandBuffer(imageIndex);
    result = sulpurSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
        sulpurWindow.wasWindowResized()) {
        sulpurWindow.resetWindowResizedFlag();
        recreateSwapChain();
    }
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

}
