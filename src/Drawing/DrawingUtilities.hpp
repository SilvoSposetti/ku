#pragma once

#include <string>

/** Base class for any SVG element
 */
namespace DrawingUtilities {

/** Utility to transform points to string
 * @param point The point
 * @return The string representation of the point
 */
std::string getPointString(const std::pair<double, double>& point);

}; // namespace DrawingUtilities
