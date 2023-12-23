/*!
 * @file
 * @brief
 */

#include "Board.hpp"
#include "Neopixel.hpp"
#include "tiny/Rtt.hpp"
#include "tiny/Timer.hpp"

using namespace tiny;
using Pa14Neopixel = Neopixel::Neopixel<Neopixel::ByteOrder::grb, Clock::gclk0_frequency, PIN_PA14>;

int main()
{
  Interrupts::disable();

  Clock::init();
  TimerGroup timer_group{ SystemTick::get_instance() };
  Watchdog watchdog{ timer_group };
  Heartbeat heartbeat{ timer_group };
  Pa14Neopixel neopixel{};

  Interrupts::enable();

  Pa14Neopixel::Color color[]{
    { 0, 9, 2 },
    { 0, 8, 3 },
    { 0, 7, 4 },
    { 0, 6, 5 },
    { 0, 5, 6 },
    { 0, 4, 7 },
    { 0, 3, 8 },
    { 0, 2, 9 },
  };
  neopixel.write(color, 8);

  Rtt::init();
  Rtt::printf("Hello, RTT!");

  while(1) {
    if(timer_group.run()) {
      Interrupts::wait_for_interrupt();
    }
  }
}
