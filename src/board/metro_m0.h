/*!
 * @file
 * @brief
 */

#ifndef metro_m0_h
#define metro_m0_h

#include "micro.h"
#include "clock.h"
#include "watchdog.h"
#include "systick.h"
#include "heartbeat_pa17.h"
#include "uart_sercom0_pa10_pa11.h"
#include "spi_sercom4_pa12_pb10_pb11.h"

#define heartbeat_init heartbeat_pa17_init
#define uart_init uart_sercom0_pa10_pa11_init
#define spi_init spi_sercom4_pa12_pb10_pb11_init

#endif
