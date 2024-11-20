#include "utilities/TemporaryDirectory.h"

#include <doctest.h>

#include <fstream>
#include <vector>

static void createFileOnDisk(const std::filesystem::path& path) {
  std::ofstream outfile(path);
  outfile << "content" << std::endl;
  outfile.close();
}

TEST_CASE("TemporaryDirectory") {

  SUBCASE("Lifetime") {
    std::vector<std::string> fileNames = {"A.txt", "b.c", "d"};
    std::filesystem::path path;
    {
      TemporaryDirectory temporaryDirectory;
      // Directory exists right after construction
      path = temporaryDirectory.path();
      CHECK(std::filesystem::exists(path));
      for (const auto& fileName : fileNames) {
        createFileOnDisk(path / fileName);
        CHECK(std::filesystem::exists(path / fileName));
      }
    }
    // Directory stops existing after destruction and all its contents to do so as well
    CHECK_FALSE(std::filesystem::exists(path));

    for (const auto& fileName : fileNames) {
      createFileOnDisk(path / fileName);
      CHECK_FALSE(std::filesystem::exists(path / fileName));
    }
  }

  SUBCASE("Multiple directories") {
    TemporaryDirectory temporaryDirectory1;
    TemporaryDirectory temporaryDirectory2;
    CHECK_NE(temporaryDirectory1.path(), temporaryDirectory2.path());
    CHECK(std::filesystem::exists(temporaryDirectory1.path()));
    CHECK(std::filesystem::exists(temporaryDirectory2.path()));
  }

  SUBCASE("Can erase directory from disk manually") {
    TemporaryDirectory temporaryDirectory;
    CHECK(std::filesystem::exists(temporaryDirectory.path()));
    CHECK(std::filesystem::remove_all(temporaryDirectory.path()));
  }
}
