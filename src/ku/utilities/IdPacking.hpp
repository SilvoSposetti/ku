#pragma once

#include <cstdint>
#include <tuple>

namespace IdPacking {

/** Linearize two positive integer IDs into a single positive integer.
 * @param x The ID in the x dimension
 * @param y The ID in the y dimension
 * @param xAmount The amount of IDs in the x-dimension
 * @param yAmount The amount of IDs in the y-dimension
 * @return The packed IDs
 */
constexpr uint32_t packId(uint32_t x, uint32_t y, [[maybe_unused]] uint32_t xAmount, uint32_t yAmount) {
  return x * yAmount + y;
}

/** Linearize three positive integer IDs into a single positive integer.
 * @param x The ID in the x dimension
 * @param y The ID in the y dimension
 * @param z The ID in the z dimension
 * @param xAmount The amount of IDs in the x-dimension
 * @param yAmount The amount of IDs in the y-dimension
 * @param zAmount The amount of IDs in the z-dimension
 * @return The packed IDs
 */
constexpr uint32_t
packId(uint32_t x, uint32_t y, uint32_t z, [[maybe_unused]] uint32_t xAmount, uint32_t yAmount, uint32_t zAmount) {
  return x * (yAmount * zAmount) + y * (zAmount) + z;
}

/** Unpacks a linearized positive integer ID into its two components
 * @param packedId The ID to unpack
 * @param xAmount The amount of IDs in the x-dimension
 * @param yAmount The amount of IDs in the y-dimension
 * @return The unpacked values [x, y]
 */
constexpr std::pair<uint32_t, uint32_t>
unpackId(uint32_t packedId, [[maybe_unused]] uint32_t xAmount, uint32_t yAmount) {
  return {
      packedId / yAmount,
      packedId % yAmount,
  };
}

/** Unpacks a linearized positive integer ID into its three components
 * @param packedId The ID to unpack
 * @param xAmount The amount of IDs in the x-dimension
 * @param yAmount The amount of IDs in the y-dimension
 * @param yAmount The amount of IDs in the z-dimension
 * @return The unpacked values [x, y, z]
 */
constexpr std::tuple<uint32_t, uint32_t, uint32_t>
unpackId(uint32_t packedId, [[maybe_unused]] uint32_t xAmount, uint32_t yAmount, uint32_t zAmount) {
  return {
      packedId / (yAmount * zAmount), // 0 <= x < xAmount
      (packedId % (yAmount * zAmount)) / zAmount, //  0 <= y < yAmount
      packedId % zAmount, //  0 <= z < zAmounts
  };
}

} // namespace IdPacking
