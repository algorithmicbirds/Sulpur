#include "sulpur_pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace Sulpur {
    
    SulpurPipeline::SulpurPipeline(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
    {
        createGraphicsPipeline(vertexShaderFile, fragmentShaderFile);
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

    void SulpurPipeline::createGraphicsPipeline(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
    {
        auto fragmentShaderCode = ReadFile(fragmentShaderFile);
        auto vertexShaderCode = ReadFile(vertexShaderFile);

        std::cout << "vertexShader code size: " << vertexShaderCode.size() << "\n";
        std::cout << "fragmentShader code size : " << fragmentShaderCode.size() << "\n";
    }
    
}
