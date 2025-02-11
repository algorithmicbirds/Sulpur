#pragma once

#include "sulpur_window.hpp"
#include "sulpur_pipeline.hpp"

namespace Sulpur {
	class App {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		App() : sulpur_window(WIDTH, HEIGHT, "Sulpur"),
		sulpur_pipeline(RESOURCE_PATH "shaders/vertex_shader.spv", RESOURCE_PATH "shaders/fragment_shader.spv") {}

		void run();
	private:
		Sulpur::SulpurWindow sulpur_window;
		Sulpur::SulpurPipeline sulpur_pipeline;
	};
}

