#include "sulpur_renderer_system.hpp"
#include <vulkan/vulkan_core.h>
#include <glm/ext/vector_float2.hpp>
#include "sulpur_pipeline.hpp"

#define GLM_FORCE_RADIANS
#define GLM_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <glm/gtc/constants.hpp>

namespace Sulpur {

struct SimplePushConstantData {

    glm::mat2 transform{1.0f};
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

// namespace Sulpur

SulpurRendererSystem::SulpurRendererSystem(SulpurDevice &device, VkRenderPass renderPass)
    : sulpurDevice{device} {
    createPipelineLayout();
    createPipeline(renderPass);
}

SulpurRendererSystem::~SulpurRendererSystem() {
    vkDestroyPipelineLayout(sulpurDevice.device(), pipelineLayout, nullptr);
}

void SulpurRendererSystem::createPipelineLayout() {
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
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
        ) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout");
    }
}

void SulpurRendererSystem::createPipeline(VkRenderPass renderPass) {
    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    SulpurPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    sulpurPipeline = std::make_unique<SulpurPipeline>(
        sulpurDevice, RESOURCE_PATH "shaders/vertex_shader.spv",
        RESOURCE_PATH "shaders/fragment_shader.spv", pipelineConfig
    );
}

void SulpurRendererSystem::renderGameObjects(
    VkCommandBuffer commandBuffer, std::vector<SulpurGameObject> &gameObjects
) {
    sulpurPipeline->bind(commandBuffer);
    for (auto &obj : gameObjects) {
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
} // namespace Sulpur
