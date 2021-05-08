/*!
 * @file
 * @brief
 */

#ifndef uart_sercom4_pb8_pb9_h
#define uart_sercom4_pb8_pb9_h

#include <stdint.h>
#include "hal/i_tiny_uart.h"

i_tiny_uart_t* uart_sercom4_pb8_pb9_init(uint32_t baud);

void uart_sercom4_pb8_pb9_run(void);

#endif
