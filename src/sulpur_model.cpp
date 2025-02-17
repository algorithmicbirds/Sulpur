#include "sulpur_model.hpp"


namespace Sulpur {


SulpurModel::SulpurModel(SulpurDevice& device, const std::vector<Vertex>& vertices)
    : sulpurDevice{device} {

    createVertexBuffer(vertices);
 }


SulpurModel::~SulpurModel() { 
    vkDestroyBuffer(sulpurDevice.device(), vertexBuffer, nullptr); 
    vkFreeMemory(sulpurDevice.device(), vertexBufferMemory, nullptr);
}

std::vector<VkVertexInputBindingDescription> SulpurModel::Vertex::getBindingDescriptions() {
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> SulpurModel::Vertex::getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);
    return attributeDescriptions;
}

void SulpurModel::draw(VkCommandBuffer commandBuffer) {
    vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}
    
void SulpurModel::bind(VkCommandBuffer commandBuffer) { 
    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
}

void SulpurModel::createVertexBuffer(const std::vector<Vertex>& vertrices) {
    vertexCount = static_cast<uint32_t>(vertrices.size());
    assert(vertexCount >= 3 && "atleat 3 vertrices needed");
    VkDeviceSize bufferSize = sizeof(vertrices[0]) * vertexCount;
    sulpurDevice.createBuffer(
        bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer,
        vertexBufferMemory);
    void* data;
    vkMapMemory(sulpurDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertrices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(sulpurDevice.device(), vertexBufferMemory);

}
}  // namespace Sulpur