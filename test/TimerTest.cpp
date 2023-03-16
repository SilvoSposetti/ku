#include "utilities/Timer.h"

#include <doctest.h>
#include <thread>
#include <vector>
using namespace std::chrono_literals;

TEST_CASE("Timer") {

  SUBCASE("Track time") {
    std::vector<std::chrono::milliseconds> waitTimes = {5ms, 3ms, 7ms};
    Timer timer;
    for (const auto& waitTime : waitTimes) {
      std::this_thread::sleep_for(waitTime);
      // Can check time also right after default-initialization
      const double secondsPassed = timer.elapsed();
      const auto timePassed = std::chrono::duration<double>(secondsPassed);
      CHECK(timePassed >= waitTime);
      // Reset
      timer.reset();
    }
  }

  SUBCASE("Construct time string") {
    CHECK(Timer::constructTimeString(0) == "00:00.000000");
    CHECK(Timer::constructTimeString(0.123456) == "00:00.123456");
    CHECK(Timer::constructTimeString(1.0) == "00:01.000000");
    CHECK(Timer::constructTimeString(1.5) == "00:01.500000");
    CHECK(Timer::constructTimeString(1.618033) == "00:01.618033");
    CHECK(Timer::constructTimeString(3.141592) == "00:03.141592");
    CHECK(Timer::constructTimeString(5.777777) == "00:05.777777");
    CHECK(Timer::constructTimeString(32.333333) == "00:32.333333");
    CHECK(Timer::constructTimeString(60.456) == "01:00.456000");
    CHECK(Timer::constructTimeString(1234.56789) == "20:34.567890");
    // Invalid time (negative)
    CHECK(Timer::constructTimeString(-.5) == "");
  }
}