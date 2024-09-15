#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace FileIo {

std::optional<std::string> read(const std::filesystem::path& path);

bool write(const std::filesystem::path& path, const std::string& text);

} // namespace FileIo