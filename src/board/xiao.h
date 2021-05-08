/*!
 * @file
 * @brief
 */

#ifndef xiao_h
#define xiao_h

#include "micro.h"
#include "clock.h"
#include "watchdog.h"
#include "systick.h"
#include "heartbeat_pa17.h"
#include "uart_sercom4_pb08_pb09.h"
#include "spi_sercom0_pa05_pa06_a07.h"

#define heartbeat_init heartbeat_pa17_init
#define uart_init uart_sercom4_pb08_pb09_init
#define spi_init spi_sercom0_pa05_pa06_a07_init

#endif
