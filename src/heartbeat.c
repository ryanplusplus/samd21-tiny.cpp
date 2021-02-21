/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include <stdbool.h>
#include "heartbeat.h"
#include "port.h"

enum {
  pin = PIN_PA17,
  half_period_in_msec = 500,
};

static struct {
  tiny_timer_t timer;
} self;

static void blink(tiny_timer_group_t* group, void* context)
{
  (void)context;
  port_pin_toggle_output_level(pin);
  tiny_timer_start(group, &self.timer, half_period_in_msec, blink, NULL);
}

void heartbeat_init(tiny_timer_group_t* timer_group)
{
  struct port_config config = {
    .direction = PORT_PIN_DIR_OUTPUT,
    .input_pull = PORT_PIN_PULL_NONE,
    .powersave = false
  };
  port_pin_set_config(pin, &config);
  tiny_timer_start(timer_group, &self.timer, half_period_in_msec, blink, NULL);
}
