#include "ItemData.hpp"

ItemData::ItemData(const std::string& constraintName, bool isPrimary, int32_t overallItemId, int32_t constraintItemId)
    : constraintName(constraintName)
    , isPrimary(isPrimary)
    , overallItemId(overallItemId)
    , constraintItemId(constraintItemId) {}
