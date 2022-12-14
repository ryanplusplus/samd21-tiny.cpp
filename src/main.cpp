/*!
 * @file
 * @brief
 */

#include "Board.hpp"
#include "tiny/Timer.hpp"
#include "tiny/Rtt.hpp"

using namespace tiny;

int main()
{
  Interrupts::disable();

  Clock::init();
  TimerGroup timer_group{SystemTick::get_instance()};
  Watchdog watchdog{timer_group};
  Heartbeat heartbeat{timer_group};

  Interrupts::enable();

  Rtt::init();
  Rtt::printf("Hello, RTT!");

  while(1) {
    if(timer_group.run()) {
      Interrupts::wait_for_interrupt();
    }
  }
}
