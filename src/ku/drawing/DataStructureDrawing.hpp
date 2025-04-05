#pragma once
#include "../constraints/AbstractConstraint.hpp"
#include "../solver/DancingCellsStructure.hpp"
#include "SvgDocument.hpp"

namespace DataStructureDrawing {
/** Creates an svg document graphically showing the exact cover matrix of the provided data structure
 * @param name The name of the document that is created
 * @param dataStructure The data structure constructed with the current constraints and field
 * @param constraints The data structure constructed with the current constraints and field
 * @return The svg document
 */
std::unique_ptr<SvgDocument> create(const std::string& name,
                                    const DancingCellsStructure& dataStructure,
                                    const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);
} // namespace DataStructureDrawing