#include "utilities/FileIo.hpp"

#include "utilities/TemporaryDirectory.hpp"

#include <doctest.h>

TEST_CASE("FileIo") {

  TemporaryDirectory directory;
  SUBCASE("Valid path") {
    const auto filePath = directory.path() / "file.txt";
    const std::string contents = "File contents";
    CHECK(FileIo::write(filePath, contents));

    const auto readContents = FileIo::read(filePath);
    CHECK(readContents);
    if (readContents) {
      CHECK_EQ(contents, readContents.value());
    }
  }

  SUBCASE("Invalid path") {
    // Write to directory
    const std::string contents = "File contents";
    CHECK_FALSE(FileIo::write(directory.path(), contents));

    // Read from directory
    CHECK_FALSE(FileIo::read(directory.path()));

    // read from non-existing file path
    const auto nonExistingFile = directory.path() / "NonExisting.txt";
    CHECK_FALSE(FileIo::read(nonExistingFile));
  }
}
