#pragma once
#include "XccElement.h"

#include <set>

/** An option of the Exact Covering with Colors problem is simply a list of its elements.
 */
struct XccOption {

  /** Constructor
   */
  XccOption(const std::set<XccElement>& elements);

  /// The ordered list of elements
  const std::set<XccElement> elements;
};
