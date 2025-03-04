#pragma once

#include "sulpur_device.hpp"

// std
#include <string>
#include <vector>

namespace Sulpur {

struct PipelineConfigInfo {
    PipelineConfigInfo() = default;
    PipelineConfigInfo(const PipelineConfigInfo &) = delete;
    PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class SulpurPipeline {
  public:
    SulpurPipeline(
        SulpurDevice &device, const std::string &vertFilepath, const std::string &fragFilepath,
        const PipelineConfigInfo &configInfo
    );
    ~SulpurPipeline();

    SulpurPipeline(const SulpurPipeline &) = delete;
    SulpurPipeline &operator=(const SulpurPipeline &) = delete;

    void bind(VkCommandBuffer commandBuffer);

    static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);

  private:
    static std::vector<char> readFile(const std::string &filepath);

    void createGraphicsPipeline(
        const std::string &vertFilepath, const std::string &fragFilepath,
        const PipelineConfigInfo &configInfo
    );

    void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

    SulpurDevice &sulpurDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};
} // namespace Sulpur
