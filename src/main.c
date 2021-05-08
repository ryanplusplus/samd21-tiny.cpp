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
#include "spi_sercom4_pa12_pb10_pb11.h"

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

  tiny_comm_t comm;
  uint8_t send_buffer[100];
  uint8_t receive_buffer[100];
  tiny_comm_init(
    &comm,
    uart_sercom0_pa10_pa11_init(19200),
    send_buffer,
    sizeof(send_buffer),
    receive_buffer,
    sizeof(receive_buffer));

  char message[] = "hello, world!";
  tiny_comm_send(&comm.interface, message, sizeof(message));

  i_tiny_spi_t* spi = spi_sercom4_pa12_pb10_pb11_init(0, 0, false, 1000000);

  uint8_t write_buffer[] = "hello, spi!";
  uint8_t read_buffer[100];
  tiny_spi_transfer(spi, write_buffer, read_buffer, sizeof(write_buffer));

  while(1) {
    tiny_timer_group_run(&timer_group);
    tiny_comm_run(&comm);
    __WFI();
  }
}
