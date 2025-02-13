#include "app.hpp"
#include <array>
#include "sulpur_pipeline.hpp"


Sulpur::App::App() {
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
}

Sulpur::App::~App() { vkDestroyPipelineLayout(sulpurDevice.device(), pipelineLayout, nullptr); }

void Sulpur::App::run() {
    while (!sulpurWindow.shouldClose()) {
        glfwPollEvents();
        drawFrame();
    }
}

void Sulpur::App::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    if (vkCreatePipelineLayout(sulpurDevice.device(), &pipelineLayoutInfo, nullptr,
                               &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout");
    }

}

void Sulpur::App::createPipeline() {

    PipelineConfigInfo pipelineConfig{};
    SulpurPipeline::defaultPipelineConfigInfo(pipelineConfig, sulpurSwapChain.width(),
                                           sulpurSwapChain.height());
    pipelineConfig.renderPass = sulpurSwapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    sulpurPipeline = std::make_unique<SulpurPipeline>(
        sulpurDevice, RESOURCE_PATH "shaders/vertex_shader.spv", RESOURCE_PATH "shaders/fragment_shader.spv", pipelineConfig
    );
}

void Sulpur::App::createCommandBuffers() { 
    commandBuffers.resize(sulpurSwapChain.imageCount());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = sulpurDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(sulpurDevice.device(), &allocInfo, commandBuffers.data()) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    for (int i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
     }

    
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = sulpurSwapChain.getRenderPass();
    renderPassInfo.framebuffer = sulpurSwapChain.getFrameBuffer(i);
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = sulpurSwapChain.getSwapChainExtent();
    
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();
    
    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    sulpurPipeline->bind(commandBuffers[i]);
    vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
    vkCmdEndRenderPass(commandBuffers[i]);
    if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
    }

}

void Sulpur::App::drawFrame() {
    uint32_t imageIndex;
    auto result = sulpurSwapChain.acquireNextImage(&imageIndex);
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }
    result = sulpurSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

}
