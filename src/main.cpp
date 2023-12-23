/*!
 * @file
 * @brief
 */

#include "Board.hpp"
#include "Neopixel.hpp"
#include "tiny/Rtt.hpp"
#include "tiny/Timer.hpp"

using namespace tiny;

int main()
{
  Interrupts::disable();

  Clock::init();
  TimerGroup timer_group{ SystemTick::get_instance() };
  Watchdog watchdog{ timer_group };
  Heartbeat heartbeat{ timer_group };
  NeopixelBff neopixel{};

  Interrupts::enable();

  NeopixelBff::Color colors[]{
    { 0, 7, 5 },
    { 0, 7, 5 },
    { 0, 7, 5 },
    { 0, 7, 5 },
    { 0, 7, 5 },
    { 0, 6, 6 },
    { 0, 6, 6 },
    { 0, 6, 6 },
    { 0, 6, 6 },
    { 0, 6, 6 },
    { 0, 5, 7 },
    { 0, 5, 7 },
    { 0, 5, 7 },
    { 0, 5, 7 },
    { 0, 5, 7 },
    { 0, 4, 8 },
    { 0, 4, 8 },
    { 0, 4, 8 },
    { 0, 4, 8 },
    { 0, 4, 8 },
    { 0, 3, 9 },
    { 0, 3, 9 },
    { 0, 3, 9 },
    { 0, 3, 9 },
    { 0, 3, 9 },
  };
  neopixel.reset();
  neopixel.write(colors);

  Rtt::init();
  Rtt::printf("Hello, RTT!");

  while(1) {
    if(timer_group.run()) {
      Interrupts::wait_for_interrupt();
    }
  }
}
