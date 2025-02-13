#include "app.hpp"

Sulpur::App::App() {
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
}

Sulpur::App::~App() { vkDestroyPipelineLayout(sulpurDevice.device(), pipelineLayout, nullptr); }

void Sulpur::App::run() {
    while (!sulpurWindow.shouldClose()) {
        glfwPollEvents();
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
    auto pipelineConfig =
        SulpurPipeline::defaultPipelineConfigInfo(sulpurSwapChain.width(), sulpurSwapChain.height());
    pipelineConfig.renderPass = sulpurSwapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    sulpurPipeline = std::make_unique<SulpurPipeline>(
        sulpurDevice, RESOURCE_PATH "shaders/vertex_shader.spv", RESOURCE_PATH "shaders/fragment_shader.spv", pipelineConfig
    );
}

void Sulpur::App::createCommandBuffers() {}

void Sulpur::App::drawFrame() {}
