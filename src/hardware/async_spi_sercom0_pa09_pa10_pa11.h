/*!
 * @file
 * @brief
 */

#ifndef async_spi_sercom0_pa09_pa10_pa11_h
#define async_spi_sercom0_pa09_pa10_pa11_h

#include <stdint.h>
#include <stdbool.h>
#include "hal/i_tiny_async_spi.h"

i_tiny_async_spi_t* async_spi_sercom0_pa09_pa10_pa11_init(bool cpol, bool cpha, bool msb_first, uint32_t baud);

#endif
