#define DOCTEST_CONFIG_IMPLEMENT

#include "KuTestArguments.hpp"

#include <algorithm>
#include <doctest.h>

int main(int argc, char** argv) {

  // Get the seed to use from the command line arguments
  int32_t seed = 0;
  {
    std::vector<std::string> arguments(argv, argv + argc);
    const std::string seedArgument = "--ku-performance-test-seed=";

    const auto foundElement = std::find_if(arguments.begin(), arguments.end(), [&](const std::string& argument) {
      return (argument.rfind(seedArgument, 0) == 0);
    });

    if (foundElement != arguments.end()) {
      const std::string element = *foundElement;
      const std::string token = element.substr(element.find('=') + 1);
      seed = std::stoi(token);
    }
  }
  KuTestArguments::seed = seed;

  // Launch doctest
  doctest::Context context;
  context.applyCommandLine(argc, argv);
  int res = context.run();
  if (context.shouldExit()) {
    return res;
  }

  return EXIT_SUCCESS;
}
