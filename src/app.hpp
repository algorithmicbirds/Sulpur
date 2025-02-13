#pragma once

#include "sulpur_window.hpp"
#include "sulpur_pipeline.hpp"
#include "sulpur_device.hpp"
#include "sulpur_swap_chain.hpp"
#include <memory>
#include <vector>

namespace Sulpur {
class App {
   public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;
    
    App();
    ~App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    void run();

   private:
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();

    Sulpur::SulpurWindow sulpurWindow{WIDTH, HEIGHT, "Sulpur"};
    SulpurDevice sulpurDevice{sulpurWindow};
    SulpurSwapChain sulpurSwapChain{sulpurDevice, sulpurWindow.getExtent()};

    std::unique_ptr<SulpurPipeline> sulpurPipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
};
}  // namespace Sulpur
