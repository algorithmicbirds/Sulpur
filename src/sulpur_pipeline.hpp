#pragma once

#include "sulpur_device.hpp"

#include <string>
#include <vector>

namespace Sulpur {

struct PipelineConfigInfo {};

class SulpurPipeline {
   public:
    SulpurPipeline(SulpurDevice& device, const std::string& vertexShaderFile,
                   const std::string& fragmentShaderFile, const PipelineConfigInfo configInfo);
    ~SulpurPipeline() {}
    SulpurPipeline(const SulpurPipeline&) = delete;
    void operator=(const SulpurPipeline&) = delete;

    static PipelineConfigInfo defaultPipelineConfigInfo(unsigned int width, unsigned int height);

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
