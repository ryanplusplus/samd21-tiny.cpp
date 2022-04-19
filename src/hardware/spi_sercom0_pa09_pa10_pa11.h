/*!
 * @file
 * @brief
 */

#ifndef spi_sercom0_pa09_pa10_pa11_h
#define spi_sercom0_pa09_pa10_pa11_h

#include <stdint.h>
#include <stdbool.h>
#include "hal/i_tiny_spi.h"

i_tiny_spi_t* spi_sercom0_pa09_pa10_pa11_init(bool cpol, bool cpha, bool msb_first, uint32_t baud);

#endif
