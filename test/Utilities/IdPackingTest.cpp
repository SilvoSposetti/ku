#include "IdPacking.hpp"

#include <doctest.h>
#include <vector>

TEST_CASE("Id Packing") {

  SUBCASE("Pack") {
    // Two components
    CHECK_EQ(IdPacking::packId(0, 0, 12, 7), 0);
    CHECK_EQ(IdPacking::packId(1, 0, 2, 2), 2);
    CHECK_EQ(IdPacking::packId(2, 5, 12, 7), 19);
    CHECK_EQ(IdPacking::packId(3, 9, 4, 13), 48);
    CHECK_EQ(IdPacking::packId(3, 16, 4, 25), 91);

    // Three components
    CHECK_EQ(IdPacking::packId(0, 0, 0, 12, 7, 5), 0);
    CHECK_EQ(IdPacking::packId(1, 0, 0, 12, 7, 5), 35);
    CHECK_EQ(IdPacking::packId(1, 1, 0, 12, 7, 5), 40);
    CHECK_EQ(IdPacking::packId(3, 4, 4, 12, 7, 5), 129);
    CHECK_EQ(IdPacking::packId(3, 4, 5, 12, 7, 5), 130);
    CHECK_EQ(IdPacking::packId(9, 5, 10, 22, 9, 13), 1128);
  }

  SUBCASE("Unpack") {
    // Two components
    CHECK_EQ(std::pair<uint32_t, uint32_t>(0, 0), IdPacking::unpackId(0, 6, 5));
    CHECK_EQ(std::pair<uint32_t, uint32_t>(0, 2), IdPacking::unpackId(2, 9, 3));
    CHECK_EQ(std::pair<uint32_t, uint32_t>(1, 5), IdPacking::unpackId(19, 4, 14));
    CHECK_EQ(std::pair<uint32_t, uint32_t>(1, 17), IdPacking::unpackId(42, 6, 25));
    CHECK_EQ(std::pair<uint32_t, uint32_t>(7, 10), IdPacking::unpackId(185, 4, 25));

    // Three components
    CHECK_EQ(std::tuple<uint32_t, uint32_t, uint32_t>(0, 0, 0), IdPacking::unpackId(0, 12, 7, 5));
    CHECK_EQ(std::tuple<uint32_t, uint32_t, uint32_t>(0, 1, 1), IdPacking::unpackId(6, 9, 7, 5));
    CHECK_EQ(std::tuple<uint32_t, uint32_t, uint32_t>(2, 5, 0), IdPacking::unpackId(95, 10, 7, 5));
    CHECK_EQ(std::tuple<uint32_t, uint32_t, uint32_t>(1, 30, 2), IdPacking::unpackId(250, 2, 32, 4));
    CHECK_EQ(std::tuple<uint32_t, uint32_t, uint32_t>(4, 0, 1), IdPacking::unpackId(121, 22, 6, 5));
    CHECK_EQ(std::tuple<uint32_t, uint32_t, uint32_t>(2, 5, 6), IdPacking::unpackId(500, 9, 7, 26));
  }

  SUBCASE("Consistency") {
    // x-, y-, and z-amounts
    std::vector<std::tuple<int32_t, int32_t, int32_t>> amounts = {
        {1, 1, 1},
        {2, 3, 3},
        {5, 9, 2},
        {23, 2, 2},
        {1, 7, 15},
    };

    for (const auto& [xAmount, yAmount, zAmount] : amounts) {
      for (int32_t x = 0; x < xAmount; x++) {
        for (int32_t y = 0; y < yAmount; y++) {
          for (int32_t z = 0; z < zAmount; z++) {
            // Pack and unpack 2 IDs
            const int32_t packed2Id = IdPacking::packId(x, y, xAmount, yAmount);
            const auto [unpacked2X, unpacked2Y] = IdPacking::unpackId(packed2Id, xAmount, yAmount);
            CHECK_EQ(x, unpacked2X);
            CHECK_EQ(y, unpacked2Y);

            // Pack and unpack 3 IDs
            const int32_t packed3Ids = IdPacking::packId(x, y, z, xAmount, yAmount, zAmount);
            const auto [unpacked3X, unpacked3Y, unpacked3Z] =
                IdPacking::unpackId(packed3Ids, xAmount, yAmount, zAmount);
            CHECK_EQ(x, unpacked3X);
            CHECK_EQ(y, unpacked3Y);
            CHECK_EQ(z, unpacked3Z);
          }
        }
      }
    }
  }
}
