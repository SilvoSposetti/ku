#pragma once

#include <string>
#include <format>

/** Base class for any SVG element
 */
namespace DrawingUtilities {

/** Utility to transform numbers to string, rounding them to some digits after the comma
 * @param number A number
 * @return The rounded number as a string
 */
std::string number(double number);

/** Prefixes a string with the provided characters such that the returend one is at least n characters long
 * @param input The input string
 * @param character the character to be used for padding
 * @param n The total amount of characters required
 * @return The input padded on the left
 */
std::string padLeft(const std::string& input, char character, int32_t n);

/** Utility to transform points to string
 * @param point The point
 * @return The string representation of the point
 */
std::string getPointString(const std::pair<double, double>& point);

}; // namespace DrawingUtilities
