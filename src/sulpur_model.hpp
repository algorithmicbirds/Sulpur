#pragma once 

#include "sulpur_device.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>

namespace Sulpur {




class SulpurModel {
      
   public:
    struct Vertex {
        glm::vec2 position;

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };
     SulpurModel(SulpurDevice& device, const std::vector<Vertex>& vertices);
    ~SulpurModel();

    SulpurModel(const SulpurModel&) = delete;
    SulpurModel& operator=(const SulpurModel&) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

   private: 
       void createVertexBuffer(const std::vector<Vertex>& vertrices);

	   SulpurDevice& sulpurDevice;
       VkBuffer vertexBuffer;
       VkDeviceMemory vertexBufferMemory;
       uint32_t vertexCount;
    };
}