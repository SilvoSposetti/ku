#pragma once
#include "../randomGenerator/RandomGenerator.h"

#include <filesystem>

/**Creates a temporary directory on disk which is auto-deleted when the object is destructed.
 * This class is not thread-safe.
 * Although very unlikely, it may throw when constructing a temporary directory.
 */
class TemporaryDirectory {
public:
  /** Constructor, creates a temporary directory on disk.
   */
  TemporaryDirectory();

  /** Destructor, deletes the temporary directory from disk.
   */
  ~TemporaryDirectory();

  /** Returns the path to the temporary directory on disk.
   * @return The path
   */
  std::filesystem::path path();

private:
  /** Utility used to create the temporary directory on disk
   * @return The path to the created directory
   */
  static std::filesystem::path createTemporaryDirectory();

  /** Utility used to generate a random directory name
   * @return A random name
   */
  static std::string generateRandomDirectoryName(RandomGenerator& randomGenerator);

private:
  /** The temporary path on disk that is managed by this object
   */
  const std::filesystem::path temporaryPath;
};