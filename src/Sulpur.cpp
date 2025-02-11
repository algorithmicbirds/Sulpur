// Sulpur.cpp : Defines the entry point for the application.
//

#include "app.hpp"
#include <cstdlib>
#include <iostream>

int main() {
    Sulpur::App app{};
    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;  
    }
    return EXIT_SUCCESS;
}
