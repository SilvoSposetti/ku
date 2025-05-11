#pragma once
#include <array>
#include <cstdint>
#include <stdexcept>

/** A fixed-size unmodifiable array with some maximum capacity and an internal smaller size.
 * Used as an Option returned by Constraints
 * @tparam T The type to store
 * @tparam N The maximum size of elements that can be stored
 */
template <typename T, std::size_t N>
class FixedCapacityArray {
public:
  /** Default constructor. Creates a completely empty array.
   */
  constexpr FixedCapacityArray()
      : count(0)
      , data(initializeData({})) {}

  /** Constructor that takes an initializer list.
   * @param initializerList The initializer list.
   */
  constexpr FixedCapacityArray(const std::initializer_list<T>& initializerList)
      : count(initializerList.size())
      , data(initializeData(initializerList)) {}

  /** Copy assignment operator.
   * @param other The other instance.
   */
  constexpr FixedCapacityArray& operator=(const FixedCapacityArray& other) noexcept {
    if (this != &other) {
      count = other.count;
      data = other.data; // std::array's assignment operator is constexpr in C++23.
    }
    return *this;
  }

  /** Default three-way-comparison operator
   * @param other The other instance.
   */
  constexpr auto operator<=>(const FixedCapacityArray& other) const = default;

  /** Retrieves the amount of elements that have been set
   * @return The amount of valid elements.
   */
  constexpr std::size_t size() const noexcept {
    return count;
  }

  /** Retrieves the maximum capacity
   * @return The maximum capacity of elements.
   */
  constexpr std::size_t capacity() const noexcept {
    return N;
  }

  /** Retrieves whether there are elements or not.
   * @return Whether there are elements or not.
   */
  constexpr bool empty() const noexcept {
    return count == 0;
  }

  /** Access an element at a certain index.
   * Throws is an element is accessed outside capacity, or outside size()
   * @return The element.
   */
  constexpr const T& operator[](std::size_t index) const {
    if (index >= size() || index >= capacity()) {
      throw std::out_of_range("Accessing index outside size");
    }
    return data[index];
  }

  /** Retrieves an iterator to the first element.
   * @return An iterator to the first element.
   */
  constexpr std::array<T, N>::const_iterator begin() const noexcept {
    return data.begin();
  }

  /** Retrieves an iterator to the last element. The one at size() - 1.
   * @return An iterator to the last element.
   */
  constexpr std::array<T, N>::const_iterator end() const noexcept {
    return data.begin() + count;
  }

private:
  /** Helper to initialize the internal data from an initialzier list.
   * @param initializerList The initializer list.
   * @return The constructed array of data
   */
  constexpr std::array<T, N> initializeData(const std::initializer_list<T>& initializerList) {
    std::array<T, N> data;
    if (initializerList.size() > N) {
      throw std::out_of_range("Initializer list exceeds maximum capacity");
    }
    if (initializerList.size() > 0) {
      std::copy(initializerList.begin(), initializerList.end(), data.begin());
    }
    return data;
  }

private:
  /** The amount of valid elements.
   */
  uint32_t count;

  /** The data.
   */
  std::array<T, N> data;
};