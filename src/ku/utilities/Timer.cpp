#include "Timer.h"

Timer::Timer()
    : startTime(std::chrono::high_resolution_clock::now()){};

void Timer::reset() {
  startTime = std::chrono::high_resolution_clock::now();
}

void Timer::printElapsed(const std::string& message) const {
  const double seconds = elapsed();
  const std::string elapsedString = constructTimeString(seconds);
  std::cout << message << " [" << elapsedString << "]" << std::endl;
}

double Timer::elapsed() const {
  return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(
             std::chrono::high_resolution_clock::now() - startTime)
      .count();
}

std::string Timer::constructTimeString(double seconds) {
  if (seconds < 0) {
    return {};
  }
  // Create left-padded minutes string
  const int32_t minutes = static_cast<int32_t>(std::floor(seconds / 60.0));
  std::string minutesString = std::to_string(minutes);
  minutesString = minutesString.size() == 1 ? "0" + minutesString : minutesString;

  // Create left-padded seconds string
  std::string secondsString = std::to_string(seconds - minutes * 60);
  secondsString = secondsString[1] == '.' ? "0" + secondsString : secondsString;

  // Right-pad the decimals
  return minutesString + ":" + secondsString;
}