/*!
 * @file
 * @brief
 */

#ifndef Watchdog_hpp
#define Watchdog_hpp

#include "tiny/Timer.hpp"

class Watchdog {
 public:
  Watchdog(tiny::TimerGroup& timer_group);

 private:
  static constexpr tiny::TimerTicks period = 10;
  tiny::Timer timer{};
};

#endif
