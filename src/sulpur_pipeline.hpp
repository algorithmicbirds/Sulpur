#pragma once

#include <string>
#include <vector>

namespace Sulpur {
	class SulpurPipeline {
	public:
		SulpurPipeline(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
	private:
		static std::vector<char> ReadFile(const std::string& filePath);
		void createGraphicsPipeline(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
	};
}