/*!
 * @file
 * @brief
 */

#include "micro.h"
#include "clock.h"
#include "systick.h"
#include "tiny_timer.h"
#include "watchdog.h"
#include "heartbeat.h"
#include "tiny_comm.h"
#include "uart_sercom0_pa10_pa11.h"

static tiny_timer_group_t timer_group;

int main(void)
{
  __disable_irq();
  {
    clock_init();
    tiny_timer_group_init(&timer_group, systick_init());
    watchdog_init(&timer_group);
    heartbeat_init(&timer_group);
  }
  __enable_irq();

  i_tiny_uart_t* uart = uart_sercom0_pa10_pa11_init(19200);
  tiny_uart_send(uart, 0xA5);

  while(1) {
    tiny_timer_group_run(&timer_group);
    __WFI();
  }
}
