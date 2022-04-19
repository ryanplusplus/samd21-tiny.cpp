/*!
 * @file
 * @brief
 */

#ifndef board_h
#define board_h

#include "sam.h"
#include "interrupts.h"
#include "clock.h"
#include "watchdog.h"
#include "systick.h"
#include "heartbeat_none.h"
#include "uart_sercom0_pa06_pa07.h"
#include "buffered_uart_sercom0_pa06_pa07.h"
#include "spi_sercom0_pa09_pa10_pa11.h"
#include "async_spi_sercom0_pa09_pa10_pa11.h"
#include "i2c_sercom1_pa16_pa17.h"

#define heartbeat_init heartbeat_none_init
#define uart_init uart_sercom0_pa06_pa07_init
#define buffered_uart_init buffered_uart_sercom0_pa06_pa07_init
#define spi_init spi_sercom0_pa09_pa10_pa11_init
#define async_spi_init async_spi_sercom0_pa09_pa10_pa11_init
#define i2c_init i2c_sercom1_pa16_pa17_init

#endif
