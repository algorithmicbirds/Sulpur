#include "sulpur_pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <stdint.h>

namespace Sulpur {
   
    SulpurPipeline::SulpurPipeline(SulpurDevice& device, const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const PipelineConfigInfo configInfo) : sulpurDevice{device}
    {
        createGraphicsPipeline(vertexShaderFile, fragmentShaderFile, configInfo);
    }

    PipelineConfigInfo SulpurPipeline::defaultPipelineConfigInfo(unsigned int width, unsigned int height)
    {
        PipelineConfigInfo configInfo{};
        return configInfo;
    }

    std::vector<char> SulpurPipeline::ReadFile(const std::string& filePath)
    {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("failed to open file: " + filePath);
        }
        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }

    void SulpurPipeline::createGraphicsPipeline(const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const PipelineConfigInfo configInfo)
    {
    }

    void SulpurPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(sulpurDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module");
        }
    }

}
