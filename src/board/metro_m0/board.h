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
#include "heartbeat_pa17.h"
#include "uart_sercom0_pa10_pa11.h"
#include "buffered_uart_sercom0_pa10_pa11.h"
#include "spi_sercom4_pa12_pb10_pb11.h"
#include "async_spi_sercom4_pa12_pb10_pb11.h"
#include "i2c_sercom3_pa22_pa23.h"

#define heartbeat_init heartbeat_pa17_init
#define uart_init uart_sercom0_pa10_pa11_init
#define buffered_uart_init buffered_uart_sercom0_pa10_pa11_init
#define spi_init spi_sercom4_pa12_pb10_pb11_init
#define async_spi_init async_spi_sercom4_pa12_pb10_pb11_init
#define i2c_init i2c_sercom3_pa22_pa23_init

#endif
