#pragma once

#include <array>
#include <cstdint>

namespace Patterns {
/** Moore pattern.
 * O O O
 * O X O
 * O O O
 */
constexpr inline std::array<std::pair<int32_t, int32_t>, 8> Moore{
    {{0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}}};

/** The Knight Pattern. The allowed moves of a Knight in chess.
 *   O   O
 * O       O
 *     X
 * O       O
 *   O   O
 */
constexpr inline std::array<std::pair<int32_t, int32_t>, 8> Knight = {
    std::make_pair(2, -1), {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}, {1, 2}, {2, 1}};
} // namespace Patterns