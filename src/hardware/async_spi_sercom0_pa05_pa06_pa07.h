/*!
 * @file
 * @brief
 */

#ifndef async_spi_sercom0_pa05_pa06_pa07_h
#define async_spi_sercom0_pa05_pa06_pa07_h

#include <stdint.h>
#include <stdbool.h>
#include "hal/i_tiny_async_spi.h"

i_tiny_async_spi_t* async_spi_sercom0_pa05_pa06_pa07_init(bool cpol, bool cpha, bool msb_first, uint32_t baud);

#endif
