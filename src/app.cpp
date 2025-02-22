#include "app.hpp"
#include <vulkan/vulkan_core.h>
#include <glm/ext/vector_float2.hpp>
#include "sulpur_renderer_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <glm/gtc/constants.hpp>

namespace Sulpur {

App::App() { loadGameObjects(); }

App::~App() {}

void App::run() {
    SulpurRendererSystem sulpurRendererSystem{
        sulpurDevice, sulpurRenderer.getSwapChainRenderPass()
    };

    while (!sulpurWindow.shouldClose()) {
        glfwPollEvents();
        if (auto commandBuffer = sulpurRenderer.beginFrame()) {
            sulpurRenderer.beginSwapChainRenderPass(commandBuffer);
            sulpurRendererSystem.renderGameObjects(commandBuffer, gameObjects);
            sulpurRenderer.endSwapChainRenderPass(commandBuffer);
            sulpurRenderer.endFrame();
        }
    }
}

void App::loadGameObjects() {
    std::vector<SulpurModel::Vertex> vertices{
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f},  {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    auto sulpurModel = std::make_shared<SulpurModel>(sulpurDevice, vertices);
    auto triangle = SulpurGameObject::createGameObject();
    triangle.model = sulpurModel;
    triangle.transform2d.translation.x = 0.8f;
    triangle.transform2d.scale = {5.0f, 5.0f};
    triangle.transform2d.rotation = glm::two_pi<float>() * .025f;
    triangle.color = {1.0f, 0.3f, 0.4f};
    gameObjects.push_back(std::move(triangle));
}

} // namespace Sulpur
