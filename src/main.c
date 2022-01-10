/*!
 * @file
 * @brief
 */

#include "board/metro_m0.h"
#include "tiny_timer.h"

static tiny_timer_group_t timer_group;

static void finished(void* context)
{
  (void)context;
  volatile int x;
  x = 4;
  (void)x;
}

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

  uint8_t write_buffer[] = { 0, 1, 2, 3, 4, 5, 6 };
  uint8_t read_buffer[sizeof(write_buffer)];
  i_tiny_async_spi_t* spi = async_spi_init(1, 0, 1, 1000000);
  tiny_async_spi_transfer(spi, write_buffer, read_buffer, sizeof(write_buffer), NULL, finished);
  read_buffer[6] = 0xFF;

  while(1) {
    if(!tiny_timer_group_run(&timer_group)) {
      interrupts_wait_for_interrupt();
    }
  }
}
