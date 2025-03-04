#pragma once

#include "sulpur_device.hpp"
#include "sulpur_game_object.hpp"
#include "sulpur_renderer.hpp"
#include "sulpur_window.hpp"

// std
#include <memory>
#include <vector>

namespace Sulpur {
class App {
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    App();
    ~App();

    App(const App &) = delete;
    App &operator=(const App &) = delete;

    void run();

  private:
    void loadGameObjects();

    SulpurWindow sulpurWindow{WIDTH, HEIGHT, "Sulpur"};
    SulpurDevice sulpurDevice{sulpurWindow};
    SulpurRenderer sulpurRenderer{sulpurWindow, sulpurDevice};

    std::vector<SulpurGameObject> gameObjects;
};
} // namespace Sulpur
