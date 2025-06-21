#pragma once
#include <algorithm>
#include <array>
#include <concepts>
#include <limits>
#include <span>
#include <stdexcept>
#include <utility>

/** A fixed-size unmodifiable array with some maximum capacity and an internal smaller size.
 * Used as an Option returned by Constraints
 * @tparam T The type to store
 * @tparam N The maximum size of elements that can be stored
 */
template <typename T, T N>
  requires std::unsigned_integral<T>
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

  /** Copy Constructor.
   * @param other The other instance.
   */
  constexpr FixedCapacityArray(const FixedCapacityArray& other)
      : count(other.count)
      , data(other.data) {}

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

  /** Construct a FixedCapacityArray from an std::array
   * @param array The array
   * @return The constructed FixedCapacityArray
   */
  static constexpr FixedCapacityArray fromArray(const std::array<T, N>& array) {
    return fromArrayImplementation(array, std::make_index_sequence<N>{});
  }

  /** operator== implementation. Elements in the array after size() are ignored.
   * @param other The other instance.
   * @return whether the two FixecCapacityArrays are equal up to the elements that they contain.
   */
  constexpr bool operator==(const FixedCapacityArray& other) const noexcept {
    if (count != other.count) {
      return false;
    }
    return std::equal(data.begin(), data.begin() + count, other.data.begin());
  }

  /** Default three-way-comparison operator
   * @param other The other instance.
   */
  constexpr auto operator<=>(const FixedCapacityArray& other) const = default;

  /** Retrieves the amount of elements that have been set
   * @return The amount of valid elements.
   */
  constexpr T size() const noexcept {
    return count;
  }

  /** Retrieves the maximum capacity
   * @return The maximum capacity of elements.
   */
  constexpr T capacity() const noexcept {
    return N;
  }

  /** Retrieves whether there are elements or not.
   * @return Whether there are elements or not.
   */
  constexpr bool empty() const noexcept {
    return count == 0;
  }

  /** Access an element at a certain index.
   * Throws is an element is accessed outside capacity(), or outside size()
   * @return The element.
   */
  constexpr const T& operator[](std::size_t index) const {
    if (index > std::numeric_limits<T>::max()) {
      throw std::out_of_range("Accessing index too large for internal size");
    }
    if (index >= size() || index >= capacity()) {
      throw std::out_of_range("Accessing index outside size");
    }
    return data[index];
  }

  /** Returns a modifiable element at a certain index.
   * Throws is an element is accessed outside capacity(), or outside size()
   * @return A reference to the element
   */
  constexpr T& operator[](std::size_t index) {
    return const_cast<T&>(std::as_const(*this)[index]);
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

  /** Creates a span over the valid elements of the array, up to its size.
   * @return A span over the elemnts of the array
   */
  constexpr std::span<const T> asSpan() const {
    return std::span<const T>(data.begin(), static_cast<std::size_t>(count));
  }

  constexpr void pushBack(const T& value) {
    if (size() == capacity()) {
      throw std::out_of_range("Not enough available capacity");
    }
    data[size()] = value;
    ++count;
  }

private:
  /** Helper to initialize the internal data from an initialzier list.
   * @param initializerList The initializer list.
   * @return The constructed array of data
   */
  constexpr std::array<T, N> initializeData(const std::initializer_list<T>& initializerList) {
    std::array<T, N> data{};
    if (static_cast<T>(initializerList.size()) > N) {
      throw std::out_of_range("Initializer list exceeds maximum capacity");
    }
    if (initializerList.size() > 0) {
      std::ranges::copy(initializerList, data.begin());
    }
    return data;
  }

  /** Helper function that unpacks an std::array into an initializer list
   * @tparam index The index
   * @param array The array
   * @param indexSequence The index sequence
   * @return The constructed FixedCapacityArray
   */
  template <std::size_t... index>
  static constexpr FixedCapacityArray
  fromArrayImplementation(const std::array<T, N>& array,
                          [[maybe_unused]] const std::index_sequence<index...>& indexSequence) {
    return FixedCapacityArray{{array[index]...}};
  }

private:
  /** The amount of valid elements.
   */
  T count = std::numeric_limits<T>::min();

  /** The data.
   */
  std::array<T, N> data;
};