// Sulpur.cpp : Defines the entry point for the application.
//

#include <cstdlib>
#include <iostream>

#include "app.hpp"

int main()
{
  Sulpur::App app{};
  try
  {
    app.run();
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
