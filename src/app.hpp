#pragma once

#include "sulpur_window.hpp"

namespace Sulpur {
	class App {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		App() : sulpur_window(WIDTH, HEIGHT, "Sulpur") {} 

		void run();
	private:
		Sulpur::SulpurWindow sulpur_window;
	};
}

