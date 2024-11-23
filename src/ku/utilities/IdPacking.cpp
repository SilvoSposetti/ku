#include "IdPacking.hpp"

uint32_t IdPacking::packId(uint32_t x, uint32_t y, [[maybe_unused]] uint32_t xAmount, uint32_t yAmount) {
  return x * yAmount + y;
}

uint32_t IdPacking::packId(
    uint32_t x, uint32_t y, uint32_t z, [[maybe_unused]] uint32_t xAmount, uint32_t yAmount, uint32_t zAmount) {
  return x * (yAmount * zAmount) + y * (zAmount) + z;
}

std::pair<uint32_t, uint32_t>
IdPacking::unpackId(uint32_t packedId, [[maybe_unused]] uint32_t xAmount, uint32_t yAmount) {
  return {
      packedId / yAmount,
      packedId % yAmount,
  };
}

std::tuple<uint32_t, uint32_t, uint32_t>
IdPacking::unpackId(uint32_t packedId, [[maybe_unused]] uint32_t xAmount, uint32_t yAmount, uint32_t zAmount) {
  return {
      packedId / (yAmount * zAmount), // 0 <= x < xAmount
      (packedId % (yAmount * zAmount)) / zAmount, //  0 <= y < yAmount
      packedId % zAmount, //  0 <= z < zAmounts
  };
}
