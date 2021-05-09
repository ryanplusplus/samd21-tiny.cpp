/*!
 * @file
 * @brief
 */

#include "board/metro_m0.h"
#include "tiny_timer.h"
#include "tiny_comm.h"

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
    uart_init(19200),
    send_buffer,
    sizeof(send_buffer),
    receive_buffer,
    sizeof(receive_buffer));
  char message[] = "hello, uart!";
  tiny_comm_send(&comm.interface, message, sizeof(message));

  i_tiny_spi_t* spi = spi_init(0, 0, false, 1000000);
  uint8_t write_buffer[] = "hello, spi!";
  uint8_t read_buffer[sizeof(write_buffer)];
  tiny_spi_transfer(spi, write_buffer, read_buffer, sizeof(write_buffer));

  i_tiny_i2c_t* i2c = i2c_init();
  const uint8_t address = 0x53;
  const uint8_t id_register[] = { 0x06, 0x00 };
  tiny_i2c_write(i2c, address, true, id_register, sizeof(id_register));
  uint8_t id[1];
  tiny_i2c_read(i2c, address, false, id, sizeof(id));

  while(1) {
    tiny_timer_group_run(&timer_group);
    tiny_comm_run(&comm);
    __WFI();
  }
}
