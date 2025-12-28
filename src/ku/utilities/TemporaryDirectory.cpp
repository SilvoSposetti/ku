#include "TemporaryDirectory.hpp"

TemporaryDirectory::TemporaryDirectory()
    : temporaryPath(createTemporaryDirectory()) {}

TemporaryDirectory::~TemporaryDirectory() {
  std::filesystem::remove_all(temporaryPath);
}

std::filesystem::path TemporaryDirectory::path() {
  return temporaryPath;
}

std::string TemporaryDirectory::generateRandomDirectoryName(RandomGenerator& randomGenerator) {
  constexpr auto base = static_cast<int32_t>(1e9);
  // Use numeric name between 1'000'000'000 and 2'000'000'000
  // Which crucially, are below the maximum of uint32_t of 2'147'483'647
  // This essentially means there are ('base'+ 1) amount of names that can be picked at random
  return "ku-tmp-" + std::to_string(randomGenerator.uniformInteger(base, 2 * base));
}

std::filesystem::path TemporaryDirectory::createTemporaryDirectory() {
  const auto temporaryDirectoryRoot = std::filesystem::temp_directory_path();
  RandomGenerator randomGenerator;
  std::filesystem::path path = temporaryDirectoryRoot / generateRandomDirectoryName(randomGenerator);
  int32_t tries = 100;
  while (std::filesystem::exists(path)) {
    if (tries <= 0) {
      throw std::runtime_error(std::string("Failed creating temporary directory after 100 tries"));
    }
    path = temporaryDirectoryRoot / generateRandomDirectoryName(randomGenerator);
    tries--;
  }

  if (!std::filesystem::exists(path)) {
    std::filesystem::create_directories(path);
  }

  return path;
}