#include "sulpur_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace Sulpur {

struct SimplePushConstantData {
    glm::mat4 transform{1.f};
    alignas(16) glm::vec3 color{};
};

SulpurRenderSystem::SulpurRenderSystem(SulpurDevice &device, VkRenderPass renderPass)
    : sulpurDevice{device} {
    createPipelineLayout();
    createPipeline(renderPass);
}

SulpurRenderSystem::~SulpurRenderSystem() {
    vkDestroyPipelineLayout(sulpurDevice.device(), pipelineLayout, nullptr);
}

void SulpurRenderSystem::createPipelineLayout() {
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(
            sulpurDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout
        ) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void SulpurRenderSystem::createPipeline(VkRenderPass renderPass) {
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

void SulpurRenderSystem::renderGameObjects(
    VkCommandBuffer commandBuffer, std::vector<SulpurGameObject> &gameObjects,
    const SulpurCamera &camera
) {
    sulpurPipeline->bind(commandBuffer);

    for (auto &obj : gameObjects) {
        obj.transform.rotation.y =
            glm::mod(obj.transform.rotation.y + 0.001f, glm::two_pi<float>());
        // obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.005f,
        // glm::two_pi<float>());

        SimplePushConstantData push{};
        push.color = obj.color;
        push.transform = camera.getProjection() * obj.transform.mat4();

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
