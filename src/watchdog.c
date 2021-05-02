/*!
 * @file
 * @brief
 */

#include "watchdog.h"
#include "wdt.h"

static tiny_timer_t timer;

static void kick(tiny_timer_group_t* _timer_group, void* context)
{
  (void)context;
  (void)_timer_group;
  wdt_reset_count();
}

void watchdog_init(tiny_timer_group_t* timer_group)
{
  struct wdt_conf config = {
    .always_on = false,
    .enable = true,
    .clock_source = GCLK_GENERATOR_1, // ULP 32KHz
    .timeout_period = WDT_PERIOD_16384CLK, // 0.5 seconds
    .window_period = WDT_PERIOD_NONE,
    .early_warning_period = WDT_PERIOD_NONE
  };
  wdt_set_config(&config);

  tiny_timer_start_periodic(timer_group, &timer, 250, kick, NULL);
}
