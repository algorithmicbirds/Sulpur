#pragma once

#include "sulpur_device.hpp"

#include <string>
#include <vector>


namespace Sulpur {

struct PipelineConfigInfo {

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineViewportStateCreateInfo viewportInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicState;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class SulpurPipeline {
   public:
    SulpurPipeline(SulpurDevice& device, const std::string& vertexShaderFile,
                   const std::string& fragmentShaderFile, const PipelineConfigInfo configInfo);
    ~SulpurPipeline();
    SulpurPipeline(const SulpurPipeline&) = delete;
    SulpurPipeline operator=(const SulpurPipeline&) = delete;
    void bind(VkCommandBuffer commandBuffer);

  static void defaultPipelineConfigInfo(
      PipelineConfigInfo& configInfo);

   private:
    static std::vector<char> ReadFile(const std::string& filePath);
    void createGraphicsPipeline(const std::string& vertexShaderFile,
                                const std::string& fragmentShaderFile,
                                const PipelineConfigInfo configInfo);
    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    SulpurDevice& sulpurDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertexShaderModule;
    VkShaderModule fragmentShaderModule;
};
}  // namespace Sulpur
