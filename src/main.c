/*!
 * @file
 * @brief
 */

#include "board/metro_m0.h"
#include "tiny_timer.h"

static tiny_timer_group_t timer_group;

int main(void)
{
  interrupts_disable();
  {
    clock_init();
    tiny_timer_group_init(&timer_group, systick_init());
    watchdog_init(&timer_group);
    heartbeat_init(&timer_group);
  }
  interrupts_enable();

  i_tiny_buffered_uart_t* uart = buffered_uart_init(19200);

  uint8_t send_buffer[] = { 5, 6, 7, 8, 9 };
  tiny_buffered_uart_send(uart, send_buffer, sizeof(send_buffer));

  while(1) {
    if(!tiny_timer_group_run(&timer_group)) {
      interrupts_wait_for_interrupt();
    }

    tiny_buffered_uart_run(uart);
  }
}
