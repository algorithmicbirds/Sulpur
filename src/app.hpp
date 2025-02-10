#pragma once

#include "sulpur_window.hpp"

namespace Sulpur {
	class App {
	public:
		static constexpr int WIDTH = 600;
		static constexpr int  HEIGHT = 800;

		void run();
	private:
		Sulpur::SulpurWindow sulpur_window(600, 300, "Sulpur");
	};
}
