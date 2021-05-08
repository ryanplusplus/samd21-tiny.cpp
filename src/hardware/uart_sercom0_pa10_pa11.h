/*!
 * @file
 * @brief
 */

#ifndef uart_sercom0_pa10_pa11_h
#define uart_sercom0_pa10_pa11_h

#include <stdint.h>
#include "hal/i_tiny_uart.h"

i_tiny_uart_t* uart_sercom0_pa10_pa11_init(uint32_t baud);

void uart_sercom0_pa10_pa11_run(void);

#endif
