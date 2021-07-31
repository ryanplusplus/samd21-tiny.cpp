/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include <stdbool.h>
#include "heartbeat_pa17.h"
#include "sam.h"

enum {
  pin = PIN_PA17,
  pin_group = pin / 32,
  pin_mask = 1 << (pin % 32),
  half_period_in_msec = 500,
};

static struct {
  tiny_timer_t timer;
} self;

static void blink(tiny_timer_group_t* group, void* context)
{
  (void)group;
  (void)context;
  PORT->Group[pin_group].OUTTGL.reg = pin_mask;
}

void heartbeat_pa17_init(tiny_timer_group_t* timer_group)
{
  PORT->Group[pin_group].DIRSET.reg = pin_mask;
  tiny_timer_start_periodic(timer_group, &self.timer, half_period_in_msec, NULL, blink);
}
