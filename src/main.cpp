/*!
 * @file
 * @brief
 */

#include "board.hpp"
#include "tiny/Timer.hpp"

using namespace tiny;

int main()
{
  Interrupts::disable();

  Clock::init();
  TimerGroup timer_group{SystemTick::get_instance()};
  Watchdog watchdog{timer_group};
  Heartbeat heartbeat{timer_group};

  Interrupts::enable();

  while(1) {
    if(timer_group.run()) {
      Interrupts::wait_for_interrupt();
    }
  }
}
