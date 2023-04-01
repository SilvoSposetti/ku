#pragma once

#include <cstdint>
#include <tuple>
#include <utility>

namespace IdPacking {

/** Linearize two positive integer IDs into a single positive integer.
 * @param x The ID in the x dimension
 * @param y The ID in the y dimension
 * @param xAmount The amount of IDs in the x-dimension
 * @param yAmount The amount of IDs in the y-dimension
 * @return The packed IDs
 */
uint32_t packId(uint32_t x, uint32_t y, uint32_t xAmount, uint32_t yAmount);

/** Linearize three positive integer IDs into a single positive integer.
 * @param x The ID in the x dimension
 * @param y The ID in the y dimension
 * @param z The ID in the z dimension
 * @param xAmount The amount of IDs in the x-dimension
 * @param yAmount The amount of IDs in the y-dimension
 * @param zAmount The amount of IDs in the z-dimension
 * @return The packed IDs
 */
uint32_t packId(uint32_t x, uint32_t y, uint32_t z, uint32_t xAmount, uint32_t yAmount, uint32_t zAmount);

/** Unpacks a linearized positive integer ID into its two components
 * @param packedId The ID to unpack
 * @param xAmount The amount of IDs in the x-dimension
 * @param yAmount The amount of IDs in the y-dimension
 * @return The unpacked values [x, y]
 */
std::pair<uint32_t, uint32_t> unpackId(uint32_t packedId, uint32_t xAmount, uint32_t yAmount);

/** Unpacks a linearized positive integer ID into its three components
 * @param packedId The ID to unpack
 * @param xAmount The amount of IDs in the x-dimension
 * @param yAmount The amount of IDs in the y-dimension
 * @param yAmount The amount of IDs in the z-dimension
 * @return The unpacked values [x, y, z]
 */
std::tuple<uint32_t, uint32_t, uint32_t>
unpackId(uint32_t packedId, uint32_t xAmount, uint32_t yAmount, uint32_t zAmount);

} // namespace IdPacking