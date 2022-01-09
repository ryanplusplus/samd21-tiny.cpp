/*!
 * @file
 * @brief
 */

#include "board/xiao.h"
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

  uint8_t send_buffer[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40 };
  tiny_buffered_uart_send(uart, send_buffer, sizeof(send_buffer));

  while(1) {
    if(!tiny_timer_group_run(&timer_group)) {
      interrupts_wait_for_interrupt();
    }

    tiny_buffered_uart_run(uart);
  }
}
