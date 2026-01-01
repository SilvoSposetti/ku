#include "FileIo.hpp"

#include <fstream>

std::optional<std::string> FileIo::read(const std::filesystem::path& path) {
  if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path)) {
    return {};
  }

  std::ifstream inputFileStream(path.string());
  return std::string((std::istreambuf_iterator<char>(inputFileStream)), (std::istreambuf_iterator<char>()));
}

bool FileIo::write(const std::filesystem::path& path, const std::string& text) {
  if (path.extension().empty()) {
    return false;
  }

  const auto parentPath = path.parent_path();
  if (!std::filesystem::exists(parentPath)) {
    const bool directoryCreated = std::filesystem::create_directories(parentPath);
    if (!directoryCreated) {
      return false;
    }
  }

  std::ofstream outfile(path);
  outfile << text;
  outfile.close();
  return true;
}
