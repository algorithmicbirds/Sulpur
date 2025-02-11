#pragma once

#include "sulpur_window.hpp"
#include "sulpur_pipeline.hpp"
#include "sulpur_device.hpp"

namespace Sulpur {
class App {
   public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();

   private:
    Sulpur::SulpurWindow sulpurWindow{WIDTH, HEIGHT, "Sulpur"};
    SulpurDevice sulpurDevice{sulpurWindow};
    Sulpur::SulpurPipeline sulpur_pipeline{
        sulpurDevice, RESOURCE_PATH "shaders/vertex_shader.spv",
        RESOURCE_PATH "shaders/fragment_shader.spv",
        SulpurPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
};
}  // namespace Sulpur
