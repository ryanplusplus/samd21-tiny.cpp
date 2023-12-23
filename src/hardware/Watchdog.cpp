/*!
 * @file
 * @brief
 */

extern "C" {
#include "sam.h"
}

#include "Clock.hpp"
#include "Watchdog.hpp"

using namespace tiny;

static_assert(Clock::gclk2_frequency == 32768, "Unexpected clock configuration");

#define write_and_sync(...)           \
  do {                                \
    __VA_ARGS__                       \
    while(WDT->STATUS.bit.SYNCBUSY) { \
    }                                 \
  } while(0)

Watchdog::Watchdog(tiny::TimerGroup& timer_group)
{
  // Enable WDT clock
  PM->APBAMASK.bit.WDT_ = 1;

  // Select GCLK2 (XOSC32K)
  write_and_sync({
    GCLK->CLKCTRL.reg =
      GCLK_CLKCTRL_CLKEN |
      GCLK_CLKCTRL_GEN_GCLK2 |
      GCLK_CLKCTRL_ID(WDT_GCLK_ID);
  });

  // Set period to 16384 ticks (~0.5 seconds at 32.758 kHz)
  write_and_sync({
    WDT->CONFIG.reg = WDT_CONFIG_PER_16K;
  });

  write_and_sync({
    WDT->CTRL.reg |= WDT_CTRL_ENABLE;
  });

  timer_group.start_periodic(
    this->timer, period, +[](void*) {
      WDT->CLEAR.reg = WDT_CLEAR_CLEAR_KEY;
    });
}
