#include "app.hpp"

#include "sulpur_camera.hpp"
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

App::App() { loadGameObjects(); }

App::~App() {}

void App::run() {
    SulpurRenderSystem sulpurRenderSystem{sulpurDevice, sulpurRenderer.getSwapChainRenderPass()};
    SulpurCamera camera{};

    while (!sulpurWindow.shouldClose()) {
        glfwPollEvents();

        float aspect = sulpurRenderer.getAspectRatio();
        // camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

        if (auto commandBuffer = sulpurRenderer.beginFrame()) {
            sulpurRenderer.beginSwapChainRenderPass(commandBuffer);

            sulpurRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);

            sulpurRenderer.endSwapChainRenderPass(commandBuffer);
            sulpurRenderer.endFrame();
        }
    }

    vkDeviceWaitIdle(sulpurDevice.device());
}

std::unique_ptr<SulpurModel> createAtomicModel(
    SulpurDevice &sulfurDevice, const glm::vec3 &positionOffset, float coreRadius,
    int totalElectrons, int totalShells, float spacingBetweenShells
) {
    std::vector<SulpurModel::Vertex> atomVertices;

    glm::vec3 corePosition = {0.0f, 0.0f, 0.0f};
    glm::vec3 coreColor = {1.0f, 0.0f, 0.0f};
    atomVertices.push_back({corePosition, coreColor});

    for (int shellIndex = 1; shellIndex <= totalShells; ++shellIndex) {
        float shellRadius = coreRadius + shellIndex * spacingBetweenShells;

        for (int electronIndex = 0; electronIndex < totalElectrons / totalShells; ++electronIndex) {
            float angle =
                (float)electronIndex / (totalElectrons / totalShells) * glm::two_pi<float>();

            float xPos = shellRadius * cos(angle);
            float yPos = shellRadius * sin(angle);
            float zPos = 0;

            glm::vec3 electronPosition = {xPos, yPos, zPos};

            glm::vec3 electronColor = {0.0f, 0.0f, 1.0f};

            atomVertices.push_back({electronPosition, electronColor});
        }
    }

    for (auto &v : atomVertices) {
        v.position += positionOffset;
    }

    return std::make_unique<SulpurModel>(sulfurDevice, atomVertices);
}

void App::loadGameObjects() {
    std::shared_ptr<SulpurModel> atomModel =
        createAtomicModel(sulpurDevice, {0.0f, 0.0f, 0.0f}, 0.7f, 12, 3, 0.1f);

    auto atom = SulpurGameObject::createGameObject();
    atom.model = atomModel;
    atom.transform.translation = {0.0f, 0.0f, 2.5f};
    atom.transform.scale = {0.5f, 0.5f, 0.5f};
    gameObjects.push_back(std::move(atom));
}

} // namespace Sulpur
