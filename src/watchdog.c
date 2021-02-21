/*!
 * @file
 * @brief
 */

#include "watchdog.h"
#include "wdt.h"

void watchdog_init(void)
{
  struct wdt_conf config = {
    .always_on = false,
    .enable = true,
    .clock_source = GCLK_GENERATOR_1, // ULP 32KHz
    .timeout_period = WDT_PERIOD_1024CLK, // 1 second
    .window_period = WDT_PERIOD_NONE,
    .early_warning_period = WDT_PERIOD_NONE
  };
  wdt_set_config(&config);
}

void watchdog_kick(void)
{
  wdt_reset_count();
}
