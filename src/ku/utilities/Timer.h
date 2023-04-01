#pragma once

#include <chrono>
#include <string>

class Timer {
public:
  /** Constructor. Starts the timer as soon as it's initialized.
   */
  Timer();

  /** Resets the internal timer.
   */
  void reset();

  /** Logs the message followed by a "[mm:ss.xxxxxx]" time indicator
   * @param message The message that identifies an action in the logs
   */
  void printElapsed(const std::string& message) const;

  /**
   * @return The amount of seconds passed from when the timer was started/restarted.
   */
  double elapsed() const;

  /** Given a precise time value in seconds, Constructs a string in the format "[mm:ss.xxxxxx]".
   * @param seconds The amount of seconds that need to be formatted into a string.
   * @return The formatted string, which has always the same length.
   */
  static std::string constructTimeString(double seconds);

private:
  /// When the timer was started
  std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};