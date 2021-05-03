/*!
 * @file
 * @brief
 */

#include "watchdog.h"
#include "samd21g18a.h"
#include "clock.h"

static_assert(clock_gclk2_frequency == 32768);

#define write_and_sync(...)           \
  do {                                \
    __VA_ARGS__                       \
    while(WDT->STATUS.bit.SYNCBUSY) { \
    }                                 \
  } while(0)

static tiny_timer_t timer;

static void kick(tiny_timer_group_t* _timer_group, void* context)
{
  (void)context;
  (void)_timer_group;
  WDT->CLEAR.reg = WDT_CLEAR_CLEAR_KEY;
}

void watchdog_init(tiny_timer_group_t* timer_group)
{
  // Enable WDT clock
  PM->APBAMASK.reg |= PM_APBAMASK_WDT;

  // Select GCLK2 (XOSC32K)
  write_and_sync({
    GCLK->CLKCTRL.reg =
      GCLK_CLKCTRL_CLKEN |
      GCLK_CLKCTRL_GEN_GCLK2 |
      GCLK_CLKCTRL_ID(WDT_GCLK_ID);
  });

  // Set period to 16384 ticks (~0.5 seconds at 32.758 kHz)
  write_and_sync({
    WDT->CONFIG.reg = WDT_CONFIG_PER_11;
  });

  write_and_sync({
    WDT->CTRL.reg |= WDT_CTRL_ENABLE;
  });

  tiny_timer_start_periodic(timer_group, &timer, 250, kick, NULL);
}
